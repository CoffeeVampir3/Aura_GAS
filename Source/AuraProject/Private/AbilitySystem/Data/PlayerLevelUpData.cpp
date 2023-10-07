#include "AbilitySystem/Data/PlayerLevelUpData.h"

int UPlayerLevelUpData::GetCurrentLevelFromXp(float CurrentExperience)
{
	return LevelUpBy(CurrentExperience, 0.0) + 1;
}

bool UPlayerLevelUpData::HasMetLevelRequirementsForNextLevel(float CurrentExperience, float CurrentLevel)
{
	const int Level = FMath::FloorToInt(CurrentLevel) - 1;

	if(Level >= LevelRequirements.Num())
	{
		return false;
	}
	
	return CurrentExperience >= LevelRequirements[Level].ExperienceRequired;
}

int UPlayerLevelUpData::LevelUpBy(float CurrentExperience, float CurrentLevel)
{
	const int Level = FMath::FloorToInt32(CurrentLevel) - 1;

	if(Level >= LevelRequirements.Num())
	{
		return 0;
	}

	int NumLevels = 0;
	for (int i = Level; i < LevelRequirements.Num(); i++)
	{
		if (CurrentExperience < LevelRequirements[i].ExperienceRequired) break;
		NumLevels++;
	}

	return NumLevels;
}
