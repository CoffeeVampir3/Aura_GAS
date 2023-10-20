

#pragma once

#include "CoreMinimal.h"
#include "GameAbilityTypes.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "BaseDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UBaseDamageAbility : public UBaseAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="Damage")
	void CauseDamage(AActor* AttackTarget, bool FriendlyFire);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromDefaults(AActor* TargetActor = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	FGameplayTag DamageTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	float DebuffChance = .2f;
	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	float DebuffDamage = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	float DebuffDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
