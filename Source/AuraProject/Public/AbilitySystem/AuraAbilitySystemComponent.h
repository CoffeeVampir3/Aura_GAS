

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/BaseAbility.h"
#include "Data/GameAbilityInfoData.h"
#include "AuraAbilitySystemComponent.generated.h"

class UPlayerLevelUpData;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_RetVal_OneParam(FGameAbilityInfo, FForEachAbility, const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void OnOwnerLevelUp(int NewLevel);

	TArray<FGameAbilityInfo> ForEachAbility(const FForEachAbility& ForEachDelegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag AbilityTag);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetAbilitySpecFromInputTag(const FGameplayTag InputTag);
	static FGameplayTag GetEquipStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	UFUNCTION(BlueprintCallable)
	void BindAbilityToInputTag(FGameplayTag AbilityTag, FGameplayTag InputTag);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoints(const FGameplayTag& AbilityTag);

	void AbilityInputTagHeld(const FGameplayTag& Tag);
	void AbilityInputTagReleased(const FGameplayTag& Tag);

	virtual void OnRep_ActivateAbilities() override;

	FEffectAssetTags EffectAssetTagsDelegate;
	FAbilitiesGiven AbilitiesGivenDelegate;
	bool bHasAbilitiesGiven;
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
