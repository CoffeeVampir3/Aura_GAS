

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UGameAttributeInfo;
struct FGameplayAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendAttributeInfoSignature, const FGameplayAttributeInfo&, Info);

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class AURAPROJECT_API UAttributeMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacks() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute|Delegate")
	FSendAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGameAttributeInfo> GameAttributeInfo;
};
