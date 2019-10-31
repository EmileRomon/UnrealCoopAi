// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetAhead.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTService_SetAhead : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SetAhead();

	FVector NextExplorationPoint(class ALeaderController *Controller);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
