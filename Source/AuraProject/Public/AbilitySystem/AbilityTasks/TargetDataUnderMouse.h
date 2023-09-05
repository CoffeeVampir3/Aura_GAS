

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks|TargetData",
		meta=(DisplayName = "TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility",
		BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidMouseTargetData;

private:
	virtual void Activate() override;

	void SendMouseCursorData() const;
};
