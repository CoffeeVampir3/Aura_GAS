

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	/** Interactable Interface */
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	/** End Interactable Interface */

	/** Combat Interface */
	virtual void Die() override;
	/** End Combat Interface */

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxHealthChanged(float NewValue);
	void BroadcastInitialAttributeValues();

	UPROPERTY(BlueprintReadOnly, Category="Combat|AnimatingStatus")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float BaseWalkSpeed = 250.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="UI|Vital|Health|Display")
	TObjectPtr<UWidgetComponent> HealthBar;
};
