


#include "AbilitySystem/GameAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(const UObject* WorldContextObject,
                                                                     const ECharacterClass CharacterClass,
                                                                     const float Level,
                                                                     UAuraAbilitySystemComponent* AbilitySystemComponent)
{
	const auto GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!GameMode) return;
	
	auto [PrimaryAttributes, ClassAbilities] = GameMode->CharacterDefaultClassInfo->GetClassDefaultInfo(CharacterClass);
	const auto AttributeEngine = GameMode->CharacterDefaultClassInfo->AttributeEngine;
	const auto Vitals = GameMode->CharacterDefaultClassInfo->BaseVitals;
	const auto Growth = GameMode->CharacterDefaultClassInfo->GrowthAttributes;
	
	auto EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());

	//The order of application is actually important, this is the order things should be initialized in
	//so that dependencies are always existent in the next step.

	const auto GrowthEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(Growth, Level, EffectContext);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GrowthEffectSpec, TAGS::DATA::Level, Level);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GrowthEffectSpec.Data.Get());
	
	const auto AttributeEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(PrimaryAttributes, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*AttributeEffectSpec.Data.Get());

	const auto EngineEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(AttributeEngine, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EngineEffectSpec.Data.Get());

	const auto VitalEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(Vitals, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalEffectSpec.Data.Get());

	for(const auto Effect : GameMode->CharacterDefaultClassInfo->CommonEffects)
	{
		const auto NewEffect = AbilitySystemComponent->MakeOutgoingSpec(Effect, Level, EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewEffect.Data.Get());
	}
}

void UGameAbilitySystemLibrary::InitializeCharacterDefaultAbilities(
	const UObject* WorldContextObject,
	const ECharacterClass CharacterClass,
	const float Level,
	UAuraAbilitySystemComponent* AbilitySystemComponent)
{
	const auto GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!GameMode) return;

	auto [PrimaryAttributes, ClassAbilities] = GameMode->CharacterDefaultClassInfo->GetClassDefaultInfo(CharacterClass);
	const auto DefaultInfo = GameMode->CharacterDefaultClassInfo;
	const auto CommonAbilities = DefaultInfo->CommonAbilities;

	for (auto Ability : CommonAbilities)
	{
		auto AbilitySpec = FGameplayAbilitySpec(Ability, Level);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	
	for (auto Ability : ClassAbilities)
	{
		auto AbilitySpec = FGameplayAbilitySpec(Ability, Level);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

bool UGameAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const auto EffectContext = static_cast<const FCustomEffectContext*>(EffectContextHandle.Get());
	return EffectContext ? EffectContext->IsBlockedHit() : false;
}

bool UGameAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const auto EffectContext = static_cast<const FCustomEffectContext*>(EffectContextHandle.Get());
    return EffectContext ? EffectContext->IsCriticalHit() : false;
}

void UGameAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool HitStatus)
{
	const auto EffectContext = static_cast<FCustomEffectContext*>(EffectContextHandle.Get());
	if(!EffectContext) return;
	EffectContext->SetIsBlockedHit(HitStatus);
}

void UGameAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool HitStatus)
{
	const auto EffectContext = static_cast<FCustomEffectContext*>(EffectContextHandle.Get());
	if(!EffectContext) return;
	EffectContext->SetIsCriticalHit(HitStatus);
}

void UGameAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors,
                                                           const TArray<AActor*>& ActorsToIgnore,
                                                           float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);
		
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetCombatAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UGameAbilitySystemLibrary::AreNotFriends(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target)
{
	const bool bIsOppositeTeam = Source->HasMatchingGameplayTag(TAGS::TEAM::Player) != Target->HasMatchingGameplayTag(TAGS::TEAM::Player);
	return bIsOppositeTeam;
}

FTaggedMontage UGameAbilitySystemLibrary::GetRandomTaggedMontage(TArray<FTaggedMontage> TaggedMontages)
{
	return TaggedMontages[FMath::RandRange(0, TaggedMontages.Num()-1)];
}
