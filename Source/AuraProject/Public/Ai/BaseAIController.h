

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class AURAPROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABaseAIController();
	UPROPERTY(EditAnywhere, Category="CATEGORY")
	TObjectPtr<UBehaviorTreeComponent> BehaviourTreeComponent;
};
