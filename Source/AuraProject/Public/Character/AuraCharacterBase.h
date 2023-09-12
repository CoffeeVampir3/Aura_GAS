

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UTaggedMontageData;
class UMotionWarpingComponent;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class UAnimMontage;

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
	
	virtual int32 GetUnitLevel_Implementation() const override;
	virtual FVector GetCombatSocketLocation_Implementation(FGameplayTag GameplayTag) override;
	virtual void SetMotionWarpingTargetFacingLocation_Implementation(const FVector WarpTargetLocation) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override {return HitReactMontage;}
	virtual AActor* GetCombatTargetActor_Implementation() override { return CombatTarget.Get();}
	virtual void SetCombatTargetActor_Implementation(AActor* TargetActor) override { CombatTarget = TargetActor;}
	virtual bool IsDead_Implementation() override;
	virtual AActor* GetCombatAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetTaggedMontageData_Implementation() override {return TaggedMontages;}
	virtual void Die() override;
	
	/*
	 *	End Combat Interface
	 */

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Class|Attributes")
	ECharacterClass CharacterClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	TArray<FTaggedMontage> TaggedMontages;

	UPROPERTY(BlueprintReadOnly, Transient, Category="Combat")
	TWeakObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	virtual void InitializeAbilityActorInfo();

	void AddCharacterAbilities() const;

	UPROPERTY(EditAnywhere, Category="Abilities|Initialization")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults|Level")
	int32 StartingLevel = 1;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;


	/*
	 * Dissolving
	 */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance,
		UMaterialInstanceDynamic* DynamicWeaponMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dissolve|SFX")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dissolve|SFX")
	TObjectPtr<UMaterialInstance> DissolveWeaponMaterialInstance;
};
