


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Logging/StructuredLog.h"
#include "Subsystems/UIControllerSubsystem.h"
#include "Subsystems/WorldCombatSystem.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(TAGS::ABILITIES::EQUIP::STATE::Equipped);
		}
		
		GiveAbility(AbilitySpec);
	}
	bHasAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::OnOwnerLevelUp(const int NewLevel)
{
	const auto CombatSubsys = this->GetWorld()->GetSubsystem<UWorldCombatSystem>();
	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		FGameplayTagContainer(TAGS::ABILITYIDS::AbilityIds), AbilitySpecs);
	
	for (auto& Info : CombatSubsys->GetGameAbilityInfo())
	{
		if(!Info.AbilityTag.IsValid()) continue;
		if(NewLevel < Info.LevelRequirement) continue;
		const auto Spec = GetAbilitySpecFromAbilityTag(Info.AbilityTag);
		if(Spec == nullptr)
		{
			//Grant ability and set it to be eligible
			auto AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(TAGS::ABILITIES::EQUIP::STATE::Eligible);
			GiveAbility(AbilitySpec);
			//Forces immediate replication on the ability (Replicated to the client.)
			MarkAbilitySpecDirty(AbilitySpec);
		}
	}
}

TArray<FGameAbilityInfo> UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& ForEachDelegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	TArray<FGameAbilityInfo> Info;
	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if(ForEachDelegate.IsBound())
		{
			const auto AbilityInfo = ForEachDelegate.Execute(AbilitySpec);
			if(!AbilityInfo.EquipStatusTag.IsValid()) continue;
			Info.Add(AbilityInfo);
		}
	}
	return Info;
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.Ability) return FGameplayTag();
	for (auto Tag : AbilitySpec.Ability.Get()->AbilityTags)
	{
		if (Tag.MatchesTag(TAGS::ABILITYIDS::AbilityIds))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		for (auto Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(AbilityTag)) return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(TAGS::INPUT::Input))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromInputTag(const FGameplayTag InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		for (auto Tag : AbilitySpec.DynamicAbilityTags)
		{
			if(Tag.MatchesTag(InputTag)) return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayTag UAuraAbilitySystemComponent::GetEquipStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if(StatusTag.MatchesTag(TAGS::ABILITIES::EQUIP::STATE::State))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::BindAbilityToInputTag(const FGameplayTag AbilityTag, const FGameplayTag InputTag)
{
	auto Spec = GetAbilitySpecFromAbilityTag(AbilityTag);
	if(!Spec) return;

	const auto OldSpecIfExisted = GetAbilitySpecFromInputTag(InputTag);
	if(OldSpecIfExisted != nullptr)
	{
		OldSpecIfExisted->DynamicAbilityTags.RemoveTag(InputTag);
	}

	for(int32 i = Spec->DynamicAbilityTags.Num()-1; i >= 0; i--)
	{
		if(const auto Tag = Spec->DynamicAbilityTags.GetByIndex(i); Tag.MatchesTag(TAGS::INPUT::Input))
        {
			UE_LOGFMT(LogTemp, Warning, "Removed Tag Named {0}", Tag.GetTagName().ToString());
        	Spec->DynamicAbilityTags.RemoveTag(Tag);
        }
	}
	
	Spec->DynamicAbilityTags.AddTag(InputTag);
	UE_LOGFMT(LogTemp, Warning, "Added Tag Named {0}", InputTag.GetTagName().ToString());

	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoints_Implementation(const FGameplayTag& AbilityTag)
{
	auto AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag);
	if(AbilitySpec == nullptr) {
		return;
	}
	
	const auto EquipStatusTag = GetEquipStatusTagFromSpec(*AbilitySpec);

	if(EquipStatusTag == TAGS::ABILITIES::EQUIP::STATE::Locked) return;

	if(GetAvatarActor()->Implements<UCombatInterface>())
	{
		if(!ICombatInterface::Execute_TrySpendSpellPoints(GetAvatarActor(), 1))
		{
			return;
		}
	} else
	{
		return;
	}

	if (EquipStatusTag == TAGS::ABILITIES::EQUIP::STATE::Equipped || EquipStatusTag == TAGS::ABILITIES::EQUIP::STATE::Unlocked) {
		AbilitySpec->Level++;
	} else if (EquipStatusTag == TAGS::ABILITIES::EQUIP::STATE::Eligible) {
		AbilitySpec->DynamicAbilityTags.RemoveTag(TAGS::ABILITIES::EQUIP::STATE::Eligible);
		AbilitySpec->DynamicAbilityTags.AddTag(TAGS::ABILITIES::EQUIP::STATE::Unlocked);
	}

	//TODO::Z
	//To be correct on client here, we need to notify the client of the actual change, so some rearchitecting would
	//be in order.
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

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bHasAbilitiesGiven)
	{
		bHasAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
