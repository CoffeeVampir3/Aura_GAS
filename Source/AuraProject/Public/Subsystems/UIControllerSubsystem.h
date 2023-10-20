

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIControllerSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FUiControllerParams
{
	GENERATED_BODY()

	FUiControllerParams() {}
	FUiControllerParams(AAuraPlayerController* PC, AAuraPlayerState* PS, UAuraAbilitySystemComponent* ASC, UAuraAttributeSet* AS)
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

USTRUCT(BlueprintType)
struct FAbilityInfoArrayWrapper
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	TArray<FGameAbilityInfo> AbilityInfoArray;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendAttributeInfoSignature, const FGameplayAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAbilityInfoArrayWrapper, Info);

class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UUIControllerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void SetWidgetControllerParams(const FUiControllerParams& WCParams);
	void BindCallbacks();

	UFUNCTION(BlueprintCallable)
	void BroadcastValues();

	UFUNCTION(BlueprintCallable)
	void OnAbilitiesGranted() const;

	UFUNCTION(BlueprintCallable)
	void OnLevelUpCheckAbilityReqs() const;
	
	UFUNCTION(BlueprintCallable)
    bool TrySpendSpellPointOnAbility(FGameplayTag AbilityTag);

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
    FAbilityInfoSignature AbilityInfoDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="GAS|Attribute|Delegate")
    FSendAttributeInfoSignature AttributeInfoDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnLevelChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnExperienceChanged;
	
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<AAuraPlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TWeakObjectPtr<UAuraAttributeSet> AttributeSet;
};
