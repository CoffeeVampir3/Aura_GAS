

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* HitImpactNiagaraEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitImpactSoundEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SkeletalComponentName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAPROJECT_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetUnitLevel() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(FGameplayTag GameplayTag);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMotionWarpingTargetFacingLocation(const FVector WarpTargetLocation) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTargetActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTargetActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetTaggedMontageData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetBloodImpactEffect();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool TryGetTaggedMontageByTag(const FGameplayTag& MontageTag, FTaggedMontage& OutTaggedMontage);

	virtual void Die() = 0;

	inline static const FName WarpMotionFacingTargetName = FName("TargetFacing");
};
