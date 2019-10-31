// Fill out your copyright notice in the Description page of Project Settings.


#include "MedicController.h"
#include "AllyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "FPSCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMedicController::AMedicController()
	:AAllyController()
{
	SightSenseComp->DetectionByAffiliation.bDetectFriendlies = true;

}

void AMedicController::UpdatePerception(const TArray<AActor*>& UpdatedActors)
{
	AlliesInSigth.Empty();
	EnemiesInSigth.Empty();
	AFPSCharacter *Self = Cast<AFPSCharacter>(GetPawn());
	if (Self)
	{
		for (AActor *act : UpdatedActors)
		{
			AFPSCharacter *Char = Cast<AFPSCharacter>(act);
			if (Char && !Char->bIsDead)
			{
				float Distance = (Char->GetActorLocation() - Self->GetActorLocation()).Size();
				if (SightSenseComp && Distance < SightSenseComp->LoseSightRadius)
				{
					if (Char->TeamColor != Self->TeamColor)
					{
						EnemiesInSigth.Add(Char);
					}
					else
					{
						AlliesInSigth.Add(Char);
					}
				}
			}

		}
	}
	bNeedToTurn = false;
}

void AMedicController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AAllyCharacter *Char = Cast<AAllyCharacter>(InPawn);

	//Init Blackboard and start behavoir tree
	if (Char && Char->BotBehavior)
	{
		SetGenericTeamId(FGenericTeamId(Char->TeamColor));

		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Target");

		AllyHealKeyID = BlackboardComp->GetKeyID("HealTarget");

		LeaderKeyID = BlackboardComp->GetKeyID("Leader");

		BehaviorComp->StartTree(*Char->BotBehavior);
	}
}