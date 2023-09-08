

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CombatInterface.generated.h"

class UAnimMontage;

UINTERFACE(MinimalAPI, NotBlueprintable, BlueprintType)
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
	UFUNCTION(BlueprintCallable)
	virtual int32 GetUnitLevel() const = 0;
	UFUNCTION(BlueprintCallable)
	virtual FVector GetCombatSocketLocation() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SetMotionWarpingTargetFacingLocation(const FVector WarpTargetLocation) const = 0;
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	inline static const FName WarpMotionFacingTargetName = FName("TargetFacing");
};
