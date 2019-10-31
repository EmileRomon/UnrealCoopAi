// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForLeader.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTService_CheckForLeader : public UBTService
{
	GENERATED_BODY()
	
protected:

	FVector ComputeDestination(class AFPSCharacter *Leader, FVector CharacterPosition);

public:
	UBTService_CheckForLeader();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
