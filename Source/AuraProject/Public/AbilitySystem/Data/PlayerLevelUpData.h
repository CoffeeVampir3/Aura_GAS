#pragma once
#include "PlayerLevelUpData.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FPlayerLevelData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExperienceRequired = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RewardedAttributePoints = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RewardedSpellPoints = 1;
};

UCLASS(BlueprintType)
class AURAPROJECT_API UPlayerLevelUpData : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	int CalculateCurrentLevelFromXP(float CurrentExperience);

	UFUNCTION(BlueprintCallable)
	float GetPercentToNextLevelFromXP(float CurrentExperience);

	UFUNCTION(BlueprintCallable)
	int GetLevelDeltaFromXP(float CurrentExperience, float CurrentLevel);
	
	UPROPERTY(EditAnywhere)
	TArray<FPlayerLevelData> LevelRequirements;
};
