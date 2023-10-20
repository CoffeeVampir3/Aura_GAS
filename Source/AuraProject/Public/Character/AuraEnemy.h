

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ABaseAIController;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/** Interactable Interface */
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	/** End Interactable Interface */

	/** Combat Interface */
	/** End Combat Interface */

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxHealthChanged(float NewValue);
	void BroadcastInitialAttributeValues();

	virtual void Die(UAbilitySystemComponent* KillerAbilitySystem) override;

	UPROPERTY(BlueprintReadOnly, Category="Combat|AnimatingStatus")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Movement")
	float BaseWalkSpeed = 250.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="UI|Vital|Health|Display")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
	
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<ABaseAIController> BaseAiController;

	FName BT_HitReactingName = FName("HitReacting");
	FName BT_RangedAttacker = FName("RangedAttacker");
	FName BT_IsDead = FName("IsDead");
};
