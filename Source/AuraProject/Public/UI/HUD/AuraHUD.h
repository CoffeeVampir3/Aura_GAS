

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/HUD.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AuraHUD.generated.h"

class UWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="ControllerClass", DynamicOutputParam="OutputController"))
	void GetWidgetController(TSubclassOf<UWidgetController> ControllerClass, UWidgetController*& OutputController);

	UWidgetController* ConstructWidgetController(TSubclassOf<UWidgetController> ControllerClass);
	
private:
	UPROPERTY()
	TObjectPtr<class UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UWidgetController>> DefaultControllersToConstruct;

	UPROPERTY()
	TMap<TSubclassOf<UWidgetController>, UWidgetController*> ConstructedControllers;
};