// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LeaderController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ALeaderController : public AAIController
{
	GENERATED_BODY()

protected:

	bool bNeedToTurn;


	UPROPERTY(Transient)
	class UBlackboardComponent *BlackboardComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent *BehaviorComp;

	UFUNCTION()
		virtual void UpdatePerception(const TArray<AActor*>& UpdatedActors);

public:
	ALeaderController();

	void TurnAround();

	FVector GetForwardVector();

	bool CanMoveForward();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
		class UAIPerceptionComponent *PerceptionComp;

	UPROPERTY(EditDefaultsOnly, Category = Perception)
		class UAISenseConfig_Sight *SightSenseComp;

	TArray<class AFPSCharacter *> EnemiesInSigth;

	virtual void OnPossess(APawn* InPawn) override;

	uint8 EnemyKeyID;

	uint8 AheadKeyID;
};
