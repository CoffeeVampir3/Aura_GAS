

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual FVector GetCombatSocketLocation() override;

protected:
	virtual void InitializeAbilityActorInfo() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
};
