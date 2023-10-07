


#include "Character/AuraCharacterBase.h"

#include "AuraGameplayTags.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraProject/AuraProject.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


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

void AAuraCharacterBase::Die()
{
    Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
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

void AAuraCharacterBase::Dissolve()
{
	const auto DynMat =
		ApplyDissolveMaterial(DissolveMaterialInstance, GetMesh(), this);
	const auto DynMatWeapon =
		ApplyDissolveMaterial(DissolveWeaponMaterialInstance, Weapon, this);
	if(!DynMat || !DynMatWeapon) return;
	StartDissolveTimeline(DynMat, DynMatWeapon);
}

