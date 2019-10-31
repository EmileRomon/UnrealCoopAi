// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Fire.h"
#include "AllyCharacter.h"
#include "AllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALeaderController *Controller = Cast<ALeaderController>(OwnerComp.GetAIOwner());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("fire")));

	if (Controller)
	{
		AFPSCharacter *Enemy = Cast<AFPSCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(Controller->EnemyKeyID));
		AAllyCharacter *Char = Cast<AAllyCharacter>(Controller->GetPawn());
		if (Char)
		{
			if (Enemy && !Enemy->bIsDead)
			{
				FVector Forward = (Enemy->GetActorLocation() - Char->GetActorLocation());
				FVector StartTmp = Char->GetActorLocation();
				StartTmp.Z += 50.0f;
				if (Char->CurrentWeapon->CanTouch(Enemy, StartTmp, Forward))
				{
					Char->FireToDirection(Forward);
					Char->StopFire();
					return EBTNodeResult::Succeeded;
				}
			}
			Char->StopFire();
			OwnerComp.GetBlackboardComponent()->ClearValue(Controller->EnemyKeyID);
		}
	}
	return EBTNodeResult::Failed;

}