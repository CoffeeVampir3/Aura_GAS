


#include "Subsystems/WidgetControllerSubsystem.h"

void UWidgetControllerSubsystem::GetWidgetController(const TSubclassOf<UObject> ControllerClass, UObject*& OutputController)
{
	if(const auto Controller = ConstructedControllers.Find(ControllerClass))
	{
		OutputController = *Controller;
		return;
	}

	//This actually is an error
	const FString DisplayName = ControllerClass->GetDisplayNameText().ToString();
	UE_LOG(LogTemp, Error, TEXT("Attempted to get a widget controller of type: %s but such a controller was never constructed"), *DisplayName);
}

void UWidgetControllerSubsystem::ConstructWidgetController(const TSubclassOf<UObject> ControllerClass,
                                                           UObject*& OutputController)
{
	UObject* NewController = NewObject<UObject>(this, ControllerClass);
    ConstructedControllers.Add(ControllerClass, NewController);

	OutputController = NewController;
}