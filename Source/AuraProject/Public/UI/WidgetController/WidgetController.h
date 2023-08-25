

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "WidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WidgetController")
	TWeakObjectPtr<UAttributeSet> AttributeSet = nullptr;
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

	virtual void BindAttributeCallbacks() {}
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAttributeSet> AttributeSet;
};
