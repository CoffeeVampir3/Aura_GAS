


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Level, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

//Clamps the BASE values to between 0 and their respective max, PRIOR to any alterations.
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

//Clamps the CURRENT values to between 0 and their respective max AFTER any alterations.
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const auto EffectProperties = MakeEffectProperties(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
		return;
	}

	if (Data.EvaluatedData.Attribute == GetMetaIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetMetaIncomingDamage();
		SetMetaIncomingDamage(0.f);

		if(LocalIncomingDamage != 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			const bool bFatal = NewHealth <= 0.f;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if(const auto PlayerController = Cast<AAuraPlayerController>(EffectProperties.SourceController);
				EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
			{
				const bool bBlocked = UGameAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
				const bool bCriticalHit = UGameAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
				PlayerController->CreateDamageNumberPopup(EffectProperties.TargetCharacter, LocalIncomingDamage,
					bCriticalHit, bBlocked);
			}

			if(!bFatal)
			{
				const auto HitReactContainer = FGameplayTagContainer(TAGS::AnimationStatus::AnimationHitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(HitReactContainer);
			} else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
			{
				CombatInterface->Die();
			}
		}
		return;
	}
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
			EffectProps.SourceController = SourceActorInfo->PlayerController.Get();
			
			if(!EffectProps.SourceController && SourceAvatarActor)
			{
				if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
				{
					EffectProps.SourceController = Pawn->GetController();
				}
			}

			if(EffectProps.SourceController)
			{
				EffectProps.SourceCharacter = Cast<ACharacter>(EffectProps.SourceController->GetPawn());
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
