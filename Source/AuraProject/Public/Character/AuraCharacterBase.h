

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

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

	virtual int32 GetUnitLevel() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	virtual void InitializeAbilityActorInfo();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	
	void InitializeDefaultEffects() const;
};
