

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

USTRUCT(BlueprintType)
struct FCombatWidgetDisplayParameters
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	float Damage;

	UPROPERTY(BlueprintReadOnly)
	bool bWasCritical;

	UPROPERTY(BlueprintReadOnly)
	bool bWasBlocked;
};

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(const FCombatWidgetDisplayParameters CombatParams);
};
