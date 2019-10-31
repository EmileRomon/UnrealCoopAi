// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeaderController.h"
#include "AllyController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AAllyController : public ALeaderController
{
	GENERATED_BODY()

public:
	AAllyController();

	virtual void OnPossess(APawn* InPawn) override;

	uint8 LeaderKeyID;

	uint8 DestinationKeyID;

};
