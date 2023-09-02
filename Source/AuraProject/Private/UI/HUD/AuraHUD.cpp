


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AAuraHUD::InitOverlay(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class was not set in the HUD class you dumb fuck."));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass.Get());
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	for(auto ControllerClass : DefaultControllersToConstruct)
	{
		auto ConstructedController = ConstructWidgetController(ControllerClass);
		check(ConstructedController);
		
		ConstructedController->SetWidgetControllerParams(WidgetControllerParams);
		ConstructedController->BindCallbacks();
		ConstructedController->BroadcastInitialValues();
	}
	
	OverlayWidget->AddToViewport();
}

void AAuraHUD::GetWidgetController(const TSubclassOf<UWidgetController> ControllerClass, UWidgetController*& OutputController)
{
	if(const auto Controller = ConstructedControllers.Find(ControllerClass))
	{
		OutputController = *Controller;
		return;
	}

	//This actually is an error
	const FString DisplayName = ControllerClass->GetDisplayNameText().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Constructed a widget controller of type: %s"), *DisplayName);
}

UWidgetController* AAuraHUD::ConstructWidgetController(TSubclassOf<UWidgetController> ControllerClass)
{
	UWidgetController* NewController = NewObject<UWidgetController>(this, ControllerClass);
	ConstructedControllers.Add(ControllerClass, NewController);
	return NewController;
}
