#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "Engine/DeveloperSettings.h"
#include "WidgetControllerDevelperSettings.generated.h"

class UDamageTextComponent;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Game Combat Settings"))
class AURAPROJECT_API UWidgetControllerDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGameAttributeInfo> GameAttributeInfo;
};
