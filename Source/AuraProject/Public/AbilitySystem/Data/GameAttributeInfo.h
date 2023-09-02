

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameAttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FGameplayAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
};

/**
 * 
 */
UCLASS(BlueprintType)
class AURAPROJECT_API UGameAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Attribute Info", meta=(ExpandBoolAsExecs="ReturnValue"))
	bool TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FGameplayAttributeInfo> AttributeInformation;
};
