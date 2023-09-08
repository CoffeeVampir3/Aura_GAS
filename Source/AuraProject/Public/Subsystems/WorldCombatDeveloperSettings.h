

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "WorldCombatDeveloperSettings.generated.h"

class UDamageTextComponent;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Game Combat Settings"))
class AURAPROJECT_API UWorldCombatDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Damage|SFX|Combat")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
