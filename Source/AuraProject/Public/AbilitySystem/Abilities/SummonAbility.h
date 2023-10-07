

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API USummonAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 SpawnThisMany = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnExclusionRadius = 140.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnRadius = 320.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpreadDegrees = 90.f;
};
