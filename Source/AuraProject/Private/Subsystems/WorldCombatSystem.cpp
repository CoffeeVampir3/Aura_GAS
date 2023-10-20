


#include "Subsystems/WorldCombatSystem.h"
#include "GameplayTags.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "Subsystems/WorldCombatDeveloperSettings.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/GameAbilityInfoData.h"

bool UWorldCombatSystem::AbilityMeetsLevelReq(const FGameplayAbilitySpec* Spec, const int CurrentLevel) const
{
	for(auto Info : GameAbilityInfo->AbilityInformation)
	{
		if(Info.Ability == Spec->Ability.GetClass())
		{
			return CurrentLevel >= Info.LevelRequirement;
		}
	}
	return false;
}

TArray<FGameAbilityInfo> UWorldCombatSystem::GetGameAbilityInfo() const
{
	return GameAbilityInfo->AbilityInformation;
}

bool UWorldCombatSystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWorldCombatSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const auto Settings = GetDefault<UWorldCombatDeveloperSettings>();
	check(Settings);
	CombatSettings = Settings;

	GameAttributeInfo = Settings->GameAttributeInfo.LoadSynchronous();
	check(GameAttributeInfo);
	GameAttributeInfo->BuildMaps();

	GameAbilityInfo = Settings->GameAbilityInfo.LoadSynchronous();
	check(GameAbilityInfo);

	CombatDamageResistanceMap.Add(TAGS::DAMAGE::TYPE::Physical, TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistancePhysical);
    CombatDamageResistanceMap.Add(TAGS::DAMAGE::TYPE::Fire, TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceFire);
    CombatDamageResistanceMap.Add(TAGS::DAMAGE::TYPE::Arcane, TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceArcane);
    CombatDamageResistanceMap.Add(TAGS::DAMAGE::TYPE::Lightning, TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceLightning);

	CombatDamageBuffMap.Add(TAGS::DAMAGE::TYPE::Physical, TAGS::BUFF::Bleeding);
	CombatDamageBuffMap.Add(TAGS::DAMAGE::TYPE::Fire, TAGS::BUFF::Burning);
	CombatDamageBuffMap.Add(TAGS::DAMAGE::TYPE::Arcane, TAGS::BUFF::Arcane);
	CombatDamageBuffMap.Add(TAGS::DAMAGE::TYPE::Lightning, TAGS::BUFF::Stun);
}

bool UWorldCombatSystem::TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo)
{
	const auto Result = GetTagToAttribInfoMap().Find(AttributeTag);
	
	if(!Result)
	{
		return false;
	}

	OutAttributeInfo = *Result;
	return true;
}

bool UWorldCombatSystem::TryGetTagInfoFromAttribute(const FGameplayAttribute& Attribute,
	FGameplayAttributeInfo& OutAttributeInfo)
{
	const auto Result = GetAttributeToInfoMap().Find(Attribute);

	if(!Result)
	{
		return false;
	}

	OutAttributeInfo = *Result;
	return true;
}

bool UWorldCombatSystem::TryGetAbilityInfoFromTag(const FGameplayTag& AbilityTag, FGameAbilityInfo& OutAbilityInfo)
{
	for(auto Info : GameAbilityInfo->AbilityInformation)
	{
		if(Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			OutAbilityInfo = Info;
			return true;
		}
	}
	return false;
}

TArray<FGameplayAttributeInfo> UWorldCombatSystem::GetAllMatchingAttributeInfoFromParentTag(FGameplayTag ParentTag) const
{
	TArray<FGameplayAttributeInfo> MatchingAttributes;
	for(auto &[Tag, AttributeInfo] : GetTagToAttribInfoMap()) {
	    if(Tag.MatchesTag(ParentTag)) {
	    	MatchingAttributes.Add(AttributeInfo);
	    }
	}
	return MatchingAttributes;
}

TMap<FGameplayTag, FGameplayTag> UWorldCombatSystem::GetCombatDamageResistanceMap()
{
	return CombatDamageResistanceMap;
}

FGameplayTag UWorldCombatSystem::GetDebuffTagFromDamageType(FGameplayTag DamageTypeTag)
{
	return *CombatDamageBuffMap.Find(DamageTypeTag);
}

FGameplayTag UWorldCombatSystem::GetDamageResistanceTag(const FGameplayTag DamageTag)
{
	return *CombatDamageResistanceMap.Find(DamageTag);
}
