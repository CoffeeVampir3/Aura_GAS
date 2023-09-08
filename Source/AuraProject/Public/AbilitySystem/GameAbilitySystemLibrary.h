

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameAbilitySystemLibrary.generated.h"

class UAuraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API UGameAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|CharacterClass|Defaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeCharacterDefaultEffects(const UObject* WorldContextObject,
		ECharacterClass CharacterClass, float Level, UAuraAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|CharacterClass|Defaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeCharacterDefaultAbilities(const UObject* WorldContextObject, float Level, UAuraAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GameplayEffects", BlueprintPure)
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GameplayEffects", BlueprintPure)
    static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool HitStatus);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool HitStatus);
};
