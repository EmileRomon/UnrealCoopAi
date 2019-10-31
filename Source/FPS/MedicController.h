// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllyController.h"
#include "MedicController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AMedicController : public AAllyController
{
	GENERATED_BODY()
	
protected:
	virtual void UpdatePerception(const TArray<AActor*>& UpdatedActors) override;

public:
	AMedicController();

	virtual void OnPossess(APawn* InPawn) override;

	TArray<class AFPSCharacter *> AlliesInSigth;

	uint8 AllyHealKeyID;
};
