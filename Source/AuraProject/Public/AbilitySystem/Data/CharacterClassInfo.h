

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UBaseAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Mage,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults|Abilities")
	TArray<TSubclassOf<UBaseAbility>> ClassAbilities;
};

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults|Primary")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassDefaultInfo;

	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults|Growth")
	TSubclassOf<UGameplayEffect> GrowthAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Vitals")
	TSubclassOf<UGameplayEffect> BaseVitals;
	
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Engine")
	TSubclassOf<UGameplayEffect> AttributeEngine;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Abilities")
	TArray<TSubclassOf<UBaseAbility>> CommonAbilities;
};
