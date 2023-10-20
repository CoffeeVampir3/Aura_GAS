


#include "Subsystems/UIControllerSubsystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/GameAbilityInfoData.h"
#include "Logging/StructuredLog.h"
#include "Subsystems/WorldCombatSystem.h"

void UUIControllerSubsystem::SetWidgetControllerParams(const FUiControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UUIControllerSubsystem::BindCallbacks()
{
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
    	
    	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
    		.AddLambda([this, AttributeInfo](const FOnAttributeChangeData& Data) mutable
    		{
    			AttributeInfo.AttributeValue = Data.NewValue;
    			AttributeInfoDelegate.Broadcast(AttributeInfo);
    		});
    }

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLevelAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnLevelChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetExperienceAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnExperienceChanged.Broadcast(Data.NewValue);
		});

	// AbilitySystemComponent->EffectAssetTagsDelegate.AddLambda(
	// 	[this](const FGameplayTagContainer& AssetTags)
	// 	{
	// 		for (auto Tag : AssetTags)
	// 		{
	// 			if (auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
	// 				!Tag.MatchesTag(MessageTag))
	// 			{
	// 				continue;
	// 			}
	// 			if (const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
	// 			{
	// 				MessageWidgetRowDelegate.Broadcast(*Row);
	// 			}
	// 		}
	// 	}
	// );
	
	if(AbilitySystemComponent->bHasAbilitiesGiven)
    {
    	OnAbilitiesGranted();
    }
    
    AbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &UUIControllerSubsystem::OnAbilitiesGranted);
}

void UUIControllerSubsystem::BroadcastValues()
{
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

void UUIControllerSubsystem::OnAbilitiesGranted() const
{
	if(!AbilitySystemComponent.IsValid()) return;

	auto CombatSubsys = this->GetWorld()->GetSubsystem<UWorldCombatSystem>();
	FForEachAbility BroadcastDelegate;
	
	BroadcastDelegate.BindLambda([this, CombatSubsys](const FGameplayAbilitySpec& AbilitySpec)->FGameAbilityInfo
	{
	    const auto AbilityTag = AbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec);
	    const auto InputTag = AbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
	    const auto EquipStatus = AbilitySystemComponent->GetEquipStatusTagFromSpec(AbilitySpec);
	    if(!AbilityTag.IsValid()) return FGameAbilityInfo();

	    FGameAbilityInfo Info;
	    CombatSubsys->TryGetAbilityInfoFromTag(AbilityTag, Info);

		Info.AbilityTag = AbilityTag;
	    Info.InputTag = InputTag;
	    Info.EquipStatusTag = EquipStatus;

		return Info;
	});

	const auto AbilityInfo = AbilitySystemComponent->ForEachAbility(BroadcastDelegate);
	BroadcastDelegate.Unbind();
	
	AbilityInfoDelegate.Broadcast(FAbilityInfoArrayWrapper(AbilityInfo));
}

void UUIControllerSubsystem::OnLevelUpCheckAbilityReqs() const
{
	OnAbilitiesGranted();
}

bool UUIControllerSubsystem::TrySpendSpellPointOnAbility(FGameplayTag AbilityTag)
{
	AbilitySystemComponent->ServerSpendSpellPoints(AbilityTag);
	OnLevelUpCheckAbilityReqs();
	return true;
}

bool UUIControllerSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
