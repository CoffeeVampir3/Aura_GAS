

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "ProjectileAbility.generated.h"

class AGameProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UProjectileAbility : public UBaseAbility
{
	GENERATED_BODY()
	
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImpactDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGameProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
