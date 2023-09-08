

#pragma once

#include "CoreMinimal.h"
#include "WorldCombatDeveloperSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/Widget/DamageTextComponent.h"
#include "WorldCombatSystem.generated.h"

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
	TSubclassOf<UWidgetComponent> GetCombatTextWidgetClass() const {return CombatSettings->DamageTextComponentClass.Get();}

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY()
	const UWorldCombatDeveloperSettings* CombatSettings;
};
