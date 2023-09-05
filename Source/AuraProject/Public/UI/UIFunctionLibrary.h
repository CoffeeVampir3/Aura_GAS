

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

class UWidgetControllerSubsystem;
class AAuraHUD;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "GAS|UI|Hud|Player Interface", BlueprintPure)
	static UWidgetControllerSubsystem* GetWidgetControllerSubsystem(UObject* WorldContextObject);
};
