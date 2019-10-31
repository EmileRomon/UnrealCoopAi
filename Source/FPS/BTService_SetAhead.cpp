// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetAhead.h"
#include "AllyCharacter.h"
#include "LeaderCharacter.h"
#include "AllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/KismetMathLibrary.h"



UBTService_SetAhead::UBTService_SetAhead()
{
	bCreateNodeInstance = true;
}

FVector UBTService_SetAhead::NextExplorationPoint(ALeaderController *Controller)
{
	FVector Ahead = FVector::ZeroVector;
	ALeaderCharacter *Char = Cast<ALeaderCharacter>(Controller->GetPawn());
	if (Char)
	{
		FVector Forward = Controller->GetForwardVector();
		float HalfRad = FMath::DegreesToRadians(70.0f);
		Forward = FMath::VRandCone(Forward, HalfRad);

		Ahead = Char->GetActorLocation() + (Forward * Char->AheadDistance);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("change next point")));
	}
	
	return Ahead;
}

void UBTService_SetAhead::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ALeaderController *Controller = Cast<ALeaderController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		FVector Ahead = NextExplorationPoint(Controller);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(Controller->AheadKeyID, Ahead);
	}
}
