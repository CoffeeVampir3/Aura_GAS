

#pragma once

#include "CoreMinimal.h"
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
	
	
};
