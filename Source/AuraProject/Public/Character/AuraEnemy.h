

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

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxHealthChanged(float NewValue);
	void BroadcastInitialAttributeValues();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="UI|Vital|Health|Display")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults|Level")
	int32 StartingLevel = 1;
};
