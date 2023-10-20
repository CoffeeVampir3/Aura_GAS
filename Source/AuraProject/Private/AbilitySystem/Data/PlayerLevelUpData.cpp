#include "AbilitySystem/Data/PlayerLevelUpData.h"

int UPlayerLevelUpData::CalculateCurrentLevelFromXP(float CurrentExperience)
{
	int NumLevels = 0;
	for (;NumLevels < LevelRequirements.Num(); NumLevels++)
	{
		if (CurrentExperience < LevelRequirements[NumLevels].ExperienceRequired) break;
	}

	return NumLevels + 1;
}

float UPlayerLevelUpData::GetPercentToNextLevelFromXP(const float CurrentExperience)
{
	float PreviousRequirement = 0.f;
    for (int NumLevels = 0;NumLevels < LevelRequirements.Num(); NumLevels++)
    {
    	const float RequiredForLevel = LevelRequirements[NumLevels].ExperienceRequired;
    	if (CurrentExperience < RequiredForLevel)
    	{
    		return (CurrentExperience-PreviousRequirement)/(RequiredForLevel - PreviousRequirement);
    	}
    	PreviousRequirement = RequiredForLevel;
    }

    return TNumericLimits<float>::Max();
}

int UPlayerLevelUpData::GetLevelDeltaFromXP(const float CurrentExperience, const float CurrentLevel)
{
	const int Level = FMath::FloorToInt(CurrentLevel);

	const int ActualLevel = CalculateCurrentLevelFromXP(CurrentExperience);

	return ActualLevel - Level;
}
