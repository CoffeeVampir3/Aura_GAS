

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "WidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerController> PlayerController = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerState> PlayerState = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<UAuraAttributeSet> AttributeSet = nullptr;
};


/**
 * 
 */
UCLASS()
class AURAPROJECT_API UWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	virtual void BroadcastInitialValues() {}

	virtual void BindCallbacks() {}
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAuraAttributeSet> AttributeSet;
};
