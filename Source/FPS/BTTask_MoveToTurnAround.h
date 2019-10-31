// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToTurnAround.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTTask_MoveToTurnAround : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:

	UBTTask_MoveToTurnAround();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
