

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UMotionWarpingComponent;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;

UCLASS(Abstract)
class AURAPROJECT_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return Cast<UAbilitySystemComponent>(AbilitySystemComponent);}
	FORCEINLINE UAuraAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/*
	 *	Combat Interface
	 */
	
	virtual int32 GetUnitLevel() const override;
	virtual FVector GetCombatSocketLocation() override {return FVector(0.f);}
	virtual void SetMotionWarpingTargetFacingLocation(const FVector WarpTargetLocation) const override;
	
	/*
	 *	End Combat Interface
	 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Class|Attributes")
	ECharacterClass CharacterClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category="CATEGORY")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	virtual void InitializeAbilityActorInfo();

	void AddCharacterAbilities() const;

	UPROPERTY(EditAnywhere, Category="Abilities|Initialization")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
