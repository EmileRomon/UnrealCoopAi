// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToTurnAround.h"
#include "LeaderController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_MoveToTurnAround::UBTTask_MoveToTurnAround()
: UBTTask_MoveTo()
{
}

EBTNodeResult::Type UBTTask_MoveToTurnAround::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	ALeaderController *Controller = Cast<ALeaderController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		Result = Super::ExecuteTask(OwnerComp, NodeMemory);
		if (Result == EBTNodeResult::Failed)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("failed")));
			Controller->TurnAround();
		}
	}
	return Result;
}
