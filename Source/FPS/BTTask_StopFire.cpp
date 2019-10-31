// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopFire.h"
#include "LeaderController.h"
#include "FPSCharacter.h"

EBTNodeResult::Type UBTTask_StopFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALeaderController *Controller = Cast<ALeaderController>(OwnerComp.GetAIOwner());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Stop fire")));

	if (Controller)
	{
		AFPSCharacter *Char = Cast<AFPSCharacter>(Controller->GetPawn());
		if (Char)
		{
			Char->StopFire();
			return EBTNodeResult::Succeeded;
		}
		
	}
	return EBTNodeResult::Failed;

}