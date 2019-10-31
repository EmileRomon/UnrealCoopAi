// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForEnnemies.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTService_CheckForEnnemies : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckForEnnemies();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
