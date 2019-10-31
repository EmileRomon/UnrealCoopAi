// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyController.h"
#include "AllyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "FPSCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAllyController::AAllyController()
:ALeaderController()
{

}

void AAllyController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AAllyCharacter *Char = Cast<AAllyCharacter>(InPawn);

	//Init Blackboard and start behavoir tree
	if (Char && Char->BotBehavior)
	{
		SetGenericTeamId(FGenericTeamId(Char->TeamColor));

		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Target");

		LeaderKeyID = BlackboardComp->GetKeyID("Leader");

		DestinationKeyID = BlackboardComp->GetKeyID("Destination");

		BehaviorComp->StartTree(*Char->BotBehavior);
	}
}