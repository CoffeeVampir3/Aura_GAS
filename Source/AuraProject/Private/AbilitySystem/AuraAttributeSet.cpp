


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(100.f);
	InitMana(150.f);
	InitMaxHealth(200.f);
	InitMaxMana(500.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

//Clamps the BASE values to between 0 and their respective max.
void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		return;
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, INFINITY);
		return;
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		return;
	}
	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, INFINITY);
		return;
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	auto EffectProperties = MakeEffectProperties(Data);
}

FEffectProperties UAuraAttributeSet::MakeEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	FEffectProperties EffectProps = FEffectProperties();
	
	EffectProps.EffectContextHandle = Data.EffectSpec.GetContext();
	const auto SourceASC = EffectProps.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(SourceASC))
	{
		EffectProps.SourceASC = SourceASC;

		const auto SourceActorInfo = SourceASC->AbilityActorInfo;
		if(SourceActorInfo.IsValid() && SourceActorInfo->AvatarActor.IsValid())
		{
			const auto SourceAvatarActor = SourceActorInfo->AvatarActor.Get();
			EffectProps.SourceAvatarActor = SourceAvatarActor;

			AController* SourceController = SourceActorInfo->PlayerController.Get();
			if(!SourceController && SourceAvatarActor)
			{
				if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
				{
					SourceController = Pawn->GetController();
				}
			}
			EffectProps.SourceController = SourceController;

			if(SourceController)
			{
				EffectProps.SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProps.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProps.TargetCharacter = Cast<ACharacter>(EffectProps.TargetAvatarActor);
		EffectProps.TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EffectProps.TargetAvatarActor);
	}

	return EffectProps;
}
