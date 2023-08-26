


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

AAuraEffectActor::AAuraEffectActor(): AActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* EffectTarget, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	bool StoreEffectApplication)
{
	const auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EffectTarget);
	if(!TargetASC) return;

	check(GameplayEffectClass);
	auto ContextHandle = TargetASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const auto EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, ContextHandle);
	auto ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	if(!StoreEffectApplication) return;
	
	auto& [GameplayEffectMap] = ActiveAppliedInfiniteEffectHandles.FindOrAdd(TargetASC);
	GameplayEffectMap.Add(GameplayEffectClass, ActiveEffectHandle);
}

void AAuraEffectActor::RemoveStoredEffectFromTarget(AActor* EffectTarget,
	TSubclassOf<UGameplayEffect> GameplayEffectClass, int Stacks)
{
	const auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EffectTarget);
	if(!TargetASC) return;

	check(GameplayEffectClass);

	if (auto TargetEffectMap = ActiveAppliedInfiniteEffectHandles.Find(TargetASC))
	{
		auto ActiveEffectHandle = TargetEffectMap->GameplayEffectMap.FindAndRemoveChecked(GameplayEffectClass);
		if(ActiveEffectHandle.IsValid())
		{
			TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle, Stacks);
			return;
		}
	}

	//The target had a valid ASC but did not have a valid effect to remove, this is probably an error.
	UE_LOG(LogTemp, Error,
		TEXT("Attempted to remove an effect from Actor: %s, GameplayEffectClass: %s but the actor did not have that effect."), 
		*EffectTarget->GetName(), *GameplayEffectClass->GetName());
}