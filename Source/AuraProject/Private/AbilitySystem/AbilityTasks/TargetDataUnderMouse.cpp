


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* ConstructedObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return ConstructedObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if(bIsLocallyControlled)
	{
		SendMouseCursorData();
	} else
	{
		const auto SpecHandle = GetAbilitySpecHandle();
		const auto ActivationKey = GetActivationPredictionKey();

		//Bind to the target data delegate.
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationKey)
		.AddLambda([this, SpecHandle, ActivationKey](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
		{
			AbilitySystemComponent->ConsumeClientReplicatedTargetData(SpecHandle, ActivationKey);
			if(ShouldBroadcastAbilityTaskDelegates())
			{
				ValidMouseTargetData.Broadcast(DataHandle);
			}
		});

		//In case our data arrived before the delegate was bound, we can ensure we retrieve the data here.
		const bool bDelegateWasCalled = AbilitySystemComponent.Get()->
			CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationKey);

		if (!bDelegateWasCalled)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	
}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	//Creates a prediction window for this function, which fills in the scoped prediction key.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	const auto PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if(!PlayerController)
		return;
	
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	const auto TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;

	DataHandle.Add(TargetData);
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if(!ShouldBroadcastAbilityTaskDelegates()) return;

	ValidMouseTargetData.Broadcast(DataHandle);
}
