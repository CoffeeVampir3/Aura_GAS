

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "BaseDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UBaseDamageAbility : public UBaseAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};