// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CkeckForAllies.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTService_CkeckForAllies : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CkeckForAllies();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
