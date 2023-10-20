


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/PlayerLevelUpData.h"
#include "AuraProject/AuraProject.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 AAuraCharacterBase::GetUnitLevel_Implementation() const
{
	if (!AttributeSet)
		return -1;

	return static_cast<int32>(AttributeSet.Get()->GetLevel());
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(FGameplayTag GameplayTag)
{
	for(auto &TaggedMontage : TaggedMontages)
	{
		if(!GameplayTag.MatchesTagExact(TaggedMontage.SocketTag)) continue;
		if(auto TaggedMeshes =
			this->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), TaggedMontage.SkeletalComponentName); TaggedMeshes.Num() > 0)
		{
			const auto Mesh = Cast<USkeletalMeshComponent>(TaggedMeshes[0]);
			return Mesh->GetSocketLocation(TaggedMontage.SocketName);
		}
	}
	return FVector::ZeroVector;
}

void AAuraCharacterBase::SetMotionWarpingTargetFacingLocation_Implementation(const FVector WarpTargetLocation) const
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpMotionFacingTargetName, WarpTargetLocation);
}

bool AAuraCharacterBase::IsDead_Implementation()
{
	if(!IsValid(AbilitySystemComponent)) return true;
	
	return AbilitySystemComponent->HasMatchingGameplayTag(TAGS::STATUS::Dead);
}

AActor* AAuraCharacterBase::GetCombatAvatar_Implementation()
{
	return this;
}

bool AAuraCharacterBase::TryGetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag, FTaggedMontage& OutTaggedMontage)
{
	for (auto TaggedMontage : TaggedMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			OutTaggedMontage = TaggedMontage;
			return true;
		}
	}
	return false;
}

void AAuraCharacterBase::OnGainedExperience(float NewExperience, float CurrentLevel)
{
	check(LevelUpDataInfo);
	check(LevelUpEffect);

	//An important note is that this does not race the client because everything here is done on the server.
	int ActualLevel = LevelUpDataInfo->CalculateCurrentLevelFromXP(NewExperience);
	int LevelDelta = LevelUpDataInfo->GetLevelDeltaFromXP(NewExperience, CurrentLevel);

	int UnitLevel = FMath::FloorToInt(CurrentLevel);
	
	if(LevelDelta <= 0) return;
	
	LevelUpLocal(ActualLevel, LevelDelta);
}

bool AAuraCharacterBase::TrySpendSpellPoints_Implementation(int NumPoints)
{
	if(NumPoints > FMath::FloorToInt(AttributeSet->GetSpellPoints()))
	{
		return false;
	}
	
	auto EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	const auto GrowthEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(ChangeAbilityPointsEffect, GetUnitLevel_Implementation(), EffectContext);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GrowthEffectSpec, TAGS::DATA::SpellPoints, static_cast<float>(-NumPoints));
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GrowthEffectSpec.Data.Get());

	return true;
}

void AAuraCharacterBase::Die(UAbilitySystemComponent* KillerAbilitySystem)
{
	// Detach From Component is Replicated already
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	
	auto EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	const auto GrowthEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(ApplyExperienceToTargetEffect, GetUnitLevel_Implementation(), EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GrowthEffectSpec.Data.Get(), KillerAbilitySystem);

	MulticastHandleDeath(FVector::UpVector * 500.f);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
    
    Weapon->SetSimulatePhysics(true);
    Weapon->SetEnableGravity(true);
    Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AbilitySystemComponent->AddLooseGameplayTag(TAGS::STATUS::Dead);

    Dissolve();
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitializeAbilityActorInfo()
{
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if(!HasAuthority()) return;

	AbilitySystemComponent->AddAbilities(StartupAbilities);
}

inline UMaterialInstanceDynamic* ApplyDissolveMaterial(UMaterialInstance* Mat, USkeletalMeshComponent* Target, UObject* Outer)
{
	if(!IsValid(Mat)) return nullptr;
	const auto DynamicMatInst = UMaterialInstanceDynamic::Create(Mat, Outer);
	Target->SetMaterial(0, DynamicMatInst);
	return DynamicMatInst;
}

void AAuraCharacterBase::MulticastOnLevelUp_Implementation()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, LevelUpSFX, GetActorLocation());
}

void AAuraCharacterBase::LevelUpLocal(const int NewLevel, const int LevelDelta)
{
	const auto Context = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle LevelUpHandle = AbilitySystemComponent->MakeOutgoingSpec(LevelUpEffect, 1.f, Context);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(LevelUpHandle, TAGS::DATA::AttributePoints, 1.0f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(LevelUpHandle, TAGS::DATA::SpellPoints, 1.0f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(LevelUpHandle, TAGS::DATA::Level, LevelDelta);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*LevelUpHandle.Data.Get());

	AbilitySystemComponent->OnOwnerLevelUp(NewLevel);

	MulticastOnLevelUp();
}

void AAuraCharacterBase::Dissolve()
{
	const auto DynMat =
		ApplyDissolveMaterial(DissolveMaterialInstance, GetMesh(), this);
	const auto DynMatWeapon =
		ApplyDissolveMaterial(DissolveWeaponMaterialInstance, Weapon, this);
	if(!DynMat || !DynMatWeapon) return;
	StartDissolveTimeline(DynMat, DynMatWeapon);
}

