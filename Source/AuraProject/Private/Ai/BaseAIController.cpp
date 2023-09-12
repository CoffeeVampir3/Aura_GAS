


#include "Ai/BaseAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


ABaseAIController::ABaseAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behaviour Tree");

	check(Blackboard);
	check(BehaviourTreeComponent);
}
