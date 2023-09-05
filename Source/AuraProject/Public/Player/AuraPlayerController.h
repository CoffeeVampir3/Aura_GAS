

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "UObject/WeakInterfacePtr.h"
#include "AuraPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UAuraAbilitySystemComponent;
class UGameInputConfiguration;
class IInteractable;
class USplineComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void Move(const struct FInputActionValue& InputActionValue);
	void ShiftPressed();
	void ShiftReleased();
	bool bShiftKeyDown;

	void CursorTrace();
	void LazyUpdateASC();
	TWeakInterfacePtr<IInteractable> LastInteractable;
	TWeakInterfacePtr<IInteractable> CurrentInteractable;

	FHitResult CursorHit;
	bool bTargeting = false;

	/*
	 * GAS
	 **/

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UGameInputConfiguration> InputConfig;

	UPROPERTY(EditAnywhere, Category="GAS|Ability System|Abilities|Input")
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/*
	 * Click-to-move
	 **/
	
	void ClickToMoveHeld();
	void ClickToMovePathed();
	void AutoRunAlongSpline();

	FVector TargetDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	bool bAutoRunning = false;

	UPROPERTY(EditDefaultsOnly, Category="Click to Move|Auto Run|Movement")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="Click to Move|Auto Run|Movement")
	float ShortPressThreshold = 0.5f;

	UPROPERTY(EditAnywhere, Category="Click to Move|Auto Run|Movement")
	TObjectPtr<USplineComponent> ClickToMoveSpline;
};
