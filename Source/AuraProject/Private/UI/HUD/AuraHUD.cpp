


#include "UI/HUD/AuraHUD.h"

#include "Subsystems/WidgetControllerSubsystem.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AAuraHUD::InitOverlay(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class was not set in the HUD class you dumb fuck."));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass.Get());

	auto WidgetControllerSubsystem = GetWorld()->GetSubsystem<UWidgetControllerSubsystem>();
	check(WidgetControllerSubsystem);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	for(auto ControllerClass : DefaultControllersToConstruct)
	{
		UObject* ConstructedController;
		WidgetControllerSubsystem->ConstructWidgetController(ControllerClass, ConstructedController);
		check(ConstructedController);

		auto CastController = Cast<UWidgetController>(ConstructedController);
		CastController->SetWidgetControllerParams(WidgetControllerParams);
		CastController->BindCallbacks();
		CastController->BroadcastInitialValues();
	}
	
	OverlayWidget->AddToViewport();
}