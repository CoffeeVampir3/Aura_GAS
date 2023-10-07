

#pragma once

#include "CoreMinimal.h"
#include "WorldCombatDeveloperSettings.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/Widget/DamageTextComponent.h"
#include "WorldCombatSystem.generated.h"

class UGameAbilityInfoData;
class UAbilityInfo;
struct FGameplayTag;
struct FGameplayAttributeInfo;
class UWidgetComponent;
class UWorldCombatDeveloperSettings;
class UDamageTextComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UWorldCombatSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo);
	UFUNCTION(BlueprintCallable)
	bool TryGetTagInfoFromAttribute(const FGameplayAttribute& Attribute, FGameplayAttributeInfo& OutAttributeInfo);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool TryGetAbilityInfoFromTag(const FGameplayTag& AttributeTag, FGameAbilityInfo& OutAttributeInfo);

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAttributeInfo> GetAllMatchingAttributeInfoFromParentTag(FGameplayTag ParentTag) const;

	FGameplayTag GetDamageResistanceTag(FGameplayTag DamageTag);
	TMap<FGameplayTag, FGameplayTag> GetCombatDamageResistanceMap();

	TSubclassOf<UWidgetComponent> GetCombatTextWidgetClass() const {return CombatSettings->DamageTextComponentClass.Get();}

	TMap<FGameplayAttribute, FGameplayAttributeInfo> GetAttributeToInfoMap() const { return GameAttributeInfo->AttributeToAttributeInfo; }
	TMap<FGameplayTag, FGameplayAttributeInfo> GetTagToAttribInfoMap() const {return GameAttributeInfo->TagToAttributeInfo;}
	UGameAttributeInfo* GetGameplayAttributeInfo() const {return GameAttributeInfo;}
	

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	const UWorldCombatDeveloperSettings* CombatSettings;

	UPROPERTY()
	TObjectPtr<UGameAttributeInfo> GameAttributeInfo;
	
	UPROPERTY()
	TObjectPtr<UGameAbilityInfoData> GameAbilityInfo;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayTag> CombatDamageResistanceMap;
};
