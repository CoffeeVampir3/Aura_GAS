


#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "AuraGameplayTags.h"

void UGameAttributeInfo::BuildMaps() {
	for (auto Info : AttributeInformation)
	{
		TagToAttributeInfo.Add(Info.AttributeTag, Info);
		AttributeToAttributeInfo.Add(Info.BindingGameplayAttribute, Info);
	}
}

bool UGameAttributeInfo::TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo)
{
	if(UNLIKELY(!bMapsBuilt))
	{
		BuildMaps();
		bMapsBuilt = true;
	}
	
	const auto Result = TagToAttributeInfo.Find(AttributeTag);
	
	if(!Result)
	{
		return false;
	}

	OutAttributeInfo = *Result;
	return true;
}

bool UGameAttributeInfo::TryGetTagInfoFromAttribute(const FGameplayAttribute& Attribute,
	FGameplayAttributeInfo& OutAttributeInfo)
{
	if(UNLIKELY(!bMapsBuilt))
	{
	    BuildMaps();
	    bMapsBuilt = true;
	}

	const auto Result = AttributeToAttributeInfo.Find(Attribute);

	if(!Result)
	{
	    return false;
	}

	OutAttributeInfo = *Result;
	return true;
}
