


#include "AbilitySystem/GameAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(const UObject* WorldContextObject,
                                                                     const ECharacterClass CharacterClass,
                                                                     const float Level,
                                                                     UAuraAbilitySystemComponent* AbilitySystemComponent)
{
	const auto GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!GameMode) return;
	
	auto [PrimaryAttributes] = GameMode->CharacterDefaultClassInfo->GetClassDefaultInfo(CharacterClass);
	const auto AttributeEngine = GameMode->CharacterDefaultClassInfo->AttributeEngine;
	const auto Vitals = GameMode->CharacterDefaultClassInfo->BaseVitals;
	
	auto EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	const auto AttributeEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(PrimaryAttributes, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*AttributeEffectSpec.Data.Get());

	const auto EngineEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(AttributeEngine, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EngineEffectSpec.Data.Get());

	const auto VitalEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(Vitals, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalEffectSpec.Data.Get());

	//TODO:: This is really fucking shit you piece of garbage, fuck you
	const auto Hack = Cast<UAuraAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass()));
	AbilitySystemComponent->SetNumericAttributeBase(Hack->GetLevelAttribute(), Level);
}
