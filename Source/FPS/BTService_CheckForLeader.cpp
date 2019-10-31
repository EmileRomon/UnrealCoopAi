// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForLeader.h"
#include "AllyCharacter.h"
#include "AllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MainCharacter.h"


UBTService_CheckForLeader::UBTService_CheckForLeader()
{
	bCreateNodeInstance = true;
}

FVector UBTService_CheckForLeader::ComputeDestination(AFPSCharacter *Leader, FVector CharacterPosition)
{
	FVector Behind = Leader->GetActorLocation() - Leader->GetVelocity().Normalize() * 200.0f;
	FVector Ahead;
	FVector Steering = Behind; //arrive
	//separation
	if (FVector::Distance(Ahead, CharacterPosition) < 100.0f)
	{
		//+= evade(leader)
	}
	if (FVector::Distance(Leader->GetActorLocation(), CharacterPosition) < 100.0f)
	{
		//+= evade(leader)
	}

	return Steering;
}

void UBTService_CheckForLeader::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AAllyController *Controller = Cast<AAllyController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		AAllyCharacter *Self = Cast<AAllyCharacter>(Controller->GetPawn());
		AFPSCharacter *Leader = Self->GetTeamLeader();

		if (Leader && Self && Leader->TeamColor == Self->TeamColor)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Controller->LeaderKeyID, Leader);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(Controller->DestinationKeyID, ComputeDestination(Leader, Self->GetActorLocation()));
		}
	}
}
