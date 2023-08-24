

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "AuraEnemy.generated.h"

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

protected:
	virtual void BeginPlay() override;
};
