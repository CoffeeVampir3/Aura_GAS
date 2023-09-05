

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameAttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FGameplayAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute BindingGameplayAttribute = FGameplayAttribute();

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
	void BuildMaps();
	
	UFUNCTION(BlueprintCallable, Category = "Attribute Info", meta=(ExpandBoolAsExecs="ReturnValue"))
	bool TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo);

	UFUNCTION(BlueprintCallable, Category = "Attribute Info", meta=(ExpandBoolAsExecs="ReturnValue"))
	bool TryGetTagInfoFromAttribute(const FGameplayAttribute& Attribute, FGameplayAttributeInfo& OutAttributeInfo);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FGameplayAttributeInfo> AttributeInformation;

	bool bMapsBuilt = false;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayAttributeInfo> TagToAttributeInfo;

	UPROPERTY()
	TMap<FGameplayAttribute, FGameplayAttributeInfo> AttributeToAttributeInfo;
};
