


#include "AbilitySystem/GameAbilitySystemGlobals.h"

#include "GameAbilityTypes.h"


FGameplayEffectContext* UGameAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCustomEffectContext();
}
