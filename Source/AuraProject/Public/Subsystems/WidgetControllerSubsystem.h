

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WidgetControllerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UWidgetControllerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="ControllerClass", DynamicOutputParam="OutputController"))
	void GetWidgetController(TSubclassOf<UObject> ControllerClass, UObject*& OutputController);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="ControllerClass", DynamicOutputParam="OutputController"))
	void ConstructWidgetController(TSubclassOf<UObject> ControllerClass, UObject*& OutputController);

	UPROPERTY()
	TMap<TSubclassOf<UObject>, UObject*> ConstructedControllers;
};
