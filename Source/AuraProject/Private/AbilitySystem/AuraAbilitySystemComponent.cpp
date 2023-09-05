


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for(const auto Ability : Abilities)
	{
	    auto AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		if(const auto AuraAbility = Cast<UBaseAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
		}
		
		GiveAbility(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;

	for (auto ActivatableSpec : GetActivatableAbilities())
	{
		AbilitySpecInputPressed(ActivatableSpec);
		if (ActivatableSpec.DynamicAbilityTags.HasTagExact(Tag) && !ActivatableSpec.IsActive())
		{
			TryActivateAbility(ActivatableSpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;

	for (auto ActivatableSpec : GetActivatableAbilities())
	{
		AbilitySpecInputReleased(ActivatableSpec);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
