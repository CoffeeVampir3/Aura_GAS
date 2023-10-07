


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/GameAbilityInfoData.h"
#include "Subsystems/WorldCombatSystem.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacks()
{
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
	
	AbilitySystemComponent->EffectAssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (auto Tag : AssetTags)
			{
				if (auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					!Tag.MatchesTag(MessageTag))
				{
					continue;
				}
				if (const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
				{
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);

	if(AbilitySystemComponent->bHasAbilitiesGiven)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got 0"));
		OnAbilitiesGranted();
	}
	
	AbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilitiesGranted);
}

void UOverlayWidgetController::OnAbilitiesGranted()
{
	if(!AbilitySystemComponent.IsValid()) return;
	
	auto CombatSubsys = this->GetWorld()->GetSubsystem<UWorldCombatSystem>();
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, CombatSubsys](const FGameplayAbilitySpec& AbilitySpec)
	{
		const auto AbilityTag = AbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec);
		const auto InputTag = AbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);

		UE_LOG(LogTemp, Warning, TEXT("A %s"), *AbilityTag.ToString());
		if(!AbilityTag.IsValid() || !InputTag.IsValid()) return;

		UE_LOG(LogTemp, Warning, TEXT("B %s"), *AbilityTag.ToString());

		FGameAbilityInfo Info;
		CombatSubsys->TryGetAbilityInfoFromTag(AbilityTag, Info);

		Info.InputTag = InputTag;
		
		AbilityInfoDelegate.Broadcast(Info);
	});

	AbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
