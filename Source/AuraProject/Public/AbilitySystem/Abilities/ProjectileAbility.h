

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageAbility.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "ProjectileAbility.generated.h"

class AGameProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UProjectileAbility : public UBaseDamageAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& CombatSocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGameProjectile> ProjectileClass;
};
