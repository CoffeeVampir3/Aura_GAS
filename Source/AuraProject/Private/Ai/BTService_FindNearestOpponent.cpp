


#include "Ai/BTService_FindNearestOpponent.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


void UBTService_FindNearestOpponent::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto OwningPawn = AIOwner->GetPawn();
	if(!IsValid(OwningPawn)) return;
	const auto TargetTeamTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTeamTag, TargetActors);

	float Closest = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for(const auto Actor : TargetActors)
	{
		if(!IsValid(Actor)) continue;

		if(const float CurrentDistance = OwningPawn->GetDistanceTo(Actor); CurrentDistance < Closest)
		{
			Closest = CurrentDistance;
			ClosestActor = Actor;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, Closest);
}
