


#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "AuraGameplayTags.h"

bool UGameAttributeInfo::TryGetTagInfo(const FGameplayTag& AttributeTag, FGameplayAttributeInfo& OutAttributeInfo)
{
	const auto Result = AttributeInformation.Find(AttributeTag);
	
	if(!Result)
	{
		return false;
	}
	
	OutAttributeInfo = *Result;
	OutAttributeInfo.AttributeTag = AttributeTag;
	return true;
}