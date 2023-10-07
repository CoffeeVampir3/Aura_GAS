


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                                const FGameplayTag& InCooldownTag)
{
	if(!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		return nullptr;
	}

	UWaitCooldownChange* Task = NewObject<UWaitCooldownChange>();
	Task->ASC = AbilitySystemComponent;
	Task->CooldownTag = InCooldownTag;

	// On Cooldown Ended. (Cooldown tag removed)
	Task->TagEventHandle = Task->ASC->RegisterGameplayTagEvent(Task->CooldownTag, EGameplayTagEventType::NewOrRemoved)
	.AddLambda([Task](const FGameplayTag Tag, int32 Count)
	{
		// Tag was removed, the cooldown has ended.
		if(Count == 0)
		{
			Task->CooldownEnd.Broadcast(0.0f);
			return;
		}
		
	});

	// On Cooldown Started. (Cooldown effect added, this is used because we need the effect info.)
	Task->EffectEventHandle = Task->ASC->OnActiveGameplayEffectAddedDelegateToSelf
	.AddLambda([Task](UAbilitySystemComponent* TargetAbilitySystem, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
	{
		FGameplayTagContainer AssetTags;
		FGameplayTagContainer GrantedTags;
		Spec.GetAllAssetTags(AssetTags);
		Spec.GetAllGrantedTags(GrantedTags);
		
		if (!AssetTags.HasTagExact(Task->CooldownTag) && !GrantedTags.HasTagExact(Task->CooldownTag)) return;

		const FGameplayEffectQuery GameplayEffectQuery =
			FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(Task->CooldownTag.GetSingleTagContainer());

		const auto CooldownTimes = Task->ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if(CooldownTimes.IsEmpty()) return;
		const auto CooldownTime = FMath::Max(CooldownTimes);
		Task->CooldownStart.Broadcast(CooldownTime);
	});
	
	return Task;
}

void UWaitCooldownChange::EndTask()
{
	if(!IsValid(ASC) || !CooldownTag.IsValid()) return;
	ASC->UnregisterGameplayTagEvent(TagEventHandle, CooldownTag, EGameplayTagEventType::NewOrRemoved);
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.Remove(EffectEventHandle);
	SetReadyToDestroy();
	MarkAsGarbage();
}
