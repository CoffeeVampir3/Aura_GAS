


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

	for(const auto& DamageTypePair : DamageMap)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,
			DamageTypePair.Key, DamageTypePair.Value.GetValueAtLevel(GetAbilityLevel()));
	}
	

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(), TargetASC);
}
