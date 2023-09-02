


#include "Character/AuraCharacterBase.h"

#include "AbilitySystem/AuraAttributeSet.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 AAuraCharacterBase::GetUnitLevel()
{
	if (!AttributeSet)
		return -1;

	return static_cast<int32>(AttributeSet.Get()->GetLevel());
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitializeAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializeDefaultEffects() const
{
	if (!IsValid(GetAbilitySystemComponent()) or InitialGameplayEffects.Num() == 0)
	{
		return;
	}

	const auto AuraASC = GetAbilitySystemComponent();
	auto EffectContext = AuraASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const auto Effect : InitialGameplayEffects)
	{
		const auto EffectSpecHandle = AuraASC->MakeOutgoingSpec(Effect, 1.0, EffectContext);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

