


#include "Subsystems/WorldCombatSystem.h"
#include "GameplayTags.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "Subsystems/WorldCombatDeveloperSettings.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/GameAbilityInfoData.h"

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

bool UWorldCombatSystem::TryGetAbilityInfoFromTag(const FGameplayTag& AbilityTag, FGameAbilityInfo& OutAttributeInfo)
{
	for(auto Info : GameAbilityInfo->AbilityInformation)
	{
		if(Info.AbilityTag == AbilityTag)
		{
			OutAttributeInfo = Info;
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

FGameplayTag UWorldCombatSystem::GetDamageResistanceTag(const FGameplayTag DamageTag)
{
	return *GetCombatDamageResistanceMap().Find(DamageTag);
}
