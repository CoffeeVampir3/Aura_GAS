

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;

USTRUCT()
struct FInnerMapStruct
{
	GENERATED_BODY()

	TMap<TSubclassOf<UGameplayEffect>, FActiveGameplayEffectHandle> GameplayEffectMap;
};

UCLASS()
class AURAPROJECT_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="GAS|Effects")
	void ApplyEffectToTarget(AActor* EffectTarget, TSubclassOf<UGameplayEffect> GameplayEffectClass, bool StoreEffectApplication = false);

	UFUNCTION(BlueprintCallable, Category="GAS|Effects")
	void RemoveStoredEffectFromTarget(AActor* EffectTarget, TSubclassOf<UGameplayEffect> GameplayEffectClass, int Stacks=-1);

	UPROPERTY()
	TMap<UAbilitySystemComponent*, FInnerMapStruct> ActiveAppliedInfiniteEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Effects")
	float ActorLevel = 1.f;
};
