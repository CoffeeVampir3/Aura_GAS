


#include "Character/AuraCharacterBase.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraProject/AuraProject.h"
#include "Components/CapsuleComponent.h"


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

int32 AAuraCharacterBase::GetUnitLevel() const
{
	if (!AttributeSet)
		return -1;

	return static_cast<int32>(AttributeSet.Get()->GetLevel());
}

void AAuraCharacterBase::SetMotionWarpingTargetFacingLocation(const FVector WarpTargetLocation) const
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpMotionFacingTargetName, WarpTargetLocation);
}

void AAuraCharacterBase::Die()
{
    Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

