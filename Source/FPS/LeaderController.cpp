// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderController.h"
#include "LeaderCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "FPSCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

void ALeaderController::UpdatePerception(const TArray<AActor*>& UpdatedActors)
{
	EnemiesInSigth.Empty();
	AFPSCharacter *Self = Cast<AFPSCharacter>(GetPawn());
	if (Self)
	{
		for (AActor *act : UpdatedActors)
		{
			AFPSCharacter *Char = Cast<AFPSCharacter>(act);
			if (Char && !Char->bIsDead && Char->TeamColor != Self->TeamColor)
			{
				float Distance = (Char->GetActorLocation() - Self->GetActorLocation()).Size();
				if (SightSenseComp && Distance < SightSenseComp->LoseSightRadius)
				{
					EnemiesInSigth.Add(Char);
				}
			}
		}
	}
	bNeedToTurn = false;
}

ALeaderController::ALeaderController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightSenseComp = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseComp"));

	//Set the perception
	if (PerceptionComp && SightSenseComp)
	{
		PerceptionComp->ConfigureSense(*SightSenseComp);
		PerceptionComp->SetDominantSense(SightSenseComp->GetSenseImplementation());

		SightSenseComp->SightRadius = 4000.0f;
		SightSenseComp->LoseSightRadius = 4500.0f;
		SightSenseComp->PeripheralVisionAngleDegrees = 180.0f;

		SightSenseComp->DetectionByAffiliation.bDetectEnemies = true;
		SightSenseComp->DetectionByAffiliation.bDetectFriendlies = false;
		SightSenseComp->DetectionByAffiliation.bDetectNeutrals = false;
	}

	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ALeaderController::UpdatePerception);
}

void ALeaderController::TurnAround()
{
	bNeedToTurn = true;
}

FVector ALeaderController::GetForwardVector()
{
	FVector Forward = GetPawn()->GetActorForwardVector();
	if (bNeedToTurn)
	{
		Forward = -Forward;
		bNeedToTurn = false;
	}
	return Forward;
}

bool ALeaderController::CanMoveForward()
{
	return !bNeedToTurn;
}

void ALeaderController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	ALeaderCharacter *Char = Cast<ALeaderCharacter>(InPawn);

	//Init Blackboard and start behavoir tree
	if (Char && Char->BotBehavior)
	{
		SetGenericTeamId(FGenericTeamId(Char->TeamColor));

		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Target");

		AheadKeyID = BlackboardComp->GetKeyID("Ahead");

		BehaviorComp->StartTree(*Char->BotBehavior);
	}

}