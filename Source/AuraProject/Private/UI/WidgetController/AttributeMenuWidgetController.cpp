


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"


void UAttributeMenuWidgetController::BindCallbacks()
{
	Super::BindCallbacks();

	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	check(GameAttributeInfo);

	FGameplayAttributeInfo AttributeInfo;
	if(bool IsInfoValid = GameAttributeInfo->TryGetTagInfo(TAGS::ATTRIBUTES::SECONDARY::Armor, AttributeInfo); !IsInfoValid)
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("Attempted to retrieve value for attribute %s in GameAttributeInfo, but it was not available."),
			*TAGS::ATTRIBUTES::SECONDARY::Armor.NAME_NativeGameplayTag.ToString()
			);
		return;
	}

	AttributeInfo.AttributeValue = AttributeSet->GetArmor();
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}
