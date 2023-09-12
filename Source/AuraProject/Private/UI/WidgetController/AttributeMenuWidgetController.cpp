


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/GameAttributeInfo.h"
#include "Subsystems/WorldCombatSystem.h"

void UAttributeMenuWidgetController::BindCallbacks()
{
	Super::BindCallbacks();
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const auto CombatSubsystem = GetWorld()->GetSubsystem<UWorldCombatSystem>();

	TArray<FGameplayAttribute> Attributes;
	AbilitySystemComponent->GetAllAttributes(Attributes);

	for (auto Attribute : Attributes)
	{
		FGameplayAttributeInfo AttributeInfo;
		if(const bool IsInfoValid = CombatSubsystem->TryGetTagInfoFromAttribute(Attribute, AttributeInfo); !IsInfoValid)
		{
			UE_LOG(LogTemp,
				Error,
				TEXT("Attempted to retrieve value for attribute %s in GameAttributeInfo, but it was not available."),
				*Attribute.GetName()
				);
			continue;
		}

		bool bFoundAttribute = false;
		const float Value = AbilitySystemComponent->GetGameplayAttributeValue(AttributeInfo.BindingGameplayAttribute, bFoundAttribute);
		if(!bFoundAttribute)
		{
			continue;
		}
		AttributeInfo.AttributeValue = Value;
		AttributeInfoDelegate.Broadcast(AttributeInfo);
	}
}
