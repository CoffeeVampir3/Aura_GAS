

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
class UAuraUserWidget;
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
	
private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UObject>> DefaultControllersToConstruct;
};