


#include "Subsystems/WorldCombatSystem.h"

#include "Subsystems/WorldCombatDeveloperSettings.h"
#include "UI/Widget/DamageTextComponent.h"

bool UWorldCombatSystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWorldCombatSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const auto Settings = GetDefault<UWorldCombatDeveloperSettings>();
	CombatSettings = Settings;
}