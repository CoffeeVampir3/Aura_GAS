


#include "AbilitySystem/Abilities/BaseDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"


void UBaseDamageAbility::CauseDamage(AActor* AttackTarget, const bool FriendlyFire)
{
	const auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AttackTarget);
	if(!TargetASC) return;

	if(!FriendlyFire)
	{
		const auto SourceASC = GetAbilitySystemComponentFromActorInfo();
		const bool bIsOppositeTeam = UGameAbilitySystemLibrary::AreNotFriends(SourceASC, TargetASC);
		if(!bIsOppositeTeam) return;
	}

	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,
	DamageTypeTag, Damage.GetValueAtLevel(GetAbilityLevel()));

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(), TargetASC);
}

FDamageEffectParams UBaseDamageAbility::MakeDamageEffectParamsFromDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageTypeTag;

	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;

	return Params;
}
