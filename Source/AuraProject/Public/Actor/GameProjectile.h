

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GameProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURAPROJECT_API AGameProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameProjectile();
	virtual void OnImpact();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Projectile|Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY()
	AActor* SourceAvatar;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY(VisibleAnywhere, Category="Collider")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category="Projectile|Niagara|Effect")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category="Projectile|Sound|Audio|SFX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Projectile|Sound|Audio|SFX")
	TObjectPtr<USoundBase> TravelingSound;

	UPROPERTY(EditAnywhere, Category="Projectile|Sound|Audio|SFX")
	TObjectPtr<UAudioComponent> TravelingSoundComponent;
};
