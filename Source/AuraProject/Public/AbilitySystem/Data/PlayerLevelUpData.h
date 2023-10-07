#pragma once

#include "PlayerLevelUpData.generated.h"

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

UCLASS()
class AURAPROJECT_API UPlayerLevelUpData : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentLevelFromXp(float CurrentExperience);
	
	UFUNCTION(BlueprintCallable)
	bool HasMetLevelRequirementsForNextLevel(float CurrentExperience, float CurrentLevel);
	
	UFUNCTION(BlueprintCallable)
	int LevelUpBy(float CurrentExperience, float CurrentLevel);
	
	UPROPERTY(EditAnywhere)
	TArray<FPlayerLevelData> LevelRequirements;
};
