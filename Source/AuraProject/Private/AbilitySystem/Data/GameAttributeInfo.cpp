


#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "AuraGameplayTags.h"

void UGameAttributeInfo::BuildMaps() {
	for (auto Info : AttributeInformation)
	{
		TagToAttributeInfo.Add(Info.AttributeTag, Info);
		AttributeToAttributeInfo.Add(Info.BindingGameplayAttribute, Info);
	}
}
