


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacks()
{
	Super::BindCallbacks();
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	check(GameAttributeInfo);

	TArray<FGameplayAttribute> Attributes;
	AbilitySystemComponent->GetAllAttributes(Attributes);

	for (auto Attribute : Attributes)
	{
		FGameplayAttributeInfo AttributeInfo;
		if(bool IsInfoValid = GameAttributeInfo->TryGetTagInfoFromAttribute(Attribute, AttributeInfo); !IsInfoValid)
		{
			UE_LOG(LogTemp,
				Error,
				TEXT("Attempted to retrieve value for attribute %s in GameAttributeInfo, but it was not available."),
				*Attribute.GetName()
				);
			continue;
		}

		bool bFoundAttribute = false;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(AttributeInfo.BindingGameplayAttribute, bFoundAttribute);
		if(!bFoundAttribute)
		{
			continue;
		}
		AttributeInfo.AttributeValue = Value;
		AttributeInfoDelegate.Broadcast(AttributeInfo);
	}
}
