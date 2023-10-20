


#include "UI/HUD/AuraHUD.h"

#include "Subsystems/UIControllerSubsystem.h"
#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::InitOverlay(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class was not set in the HUD class you dumb fuck."));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass.Get());
	
	auto UiController = GetWorld()->GetSubsystem<UUIControllerSubsystem>();
	check(UiController);
	
	const FUiControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UiController->SetWidgetControllerParams(WidgetControllerParams);
	UiController->BindCallbacks();
	
	OverlayWidget->AddToViewport();
}