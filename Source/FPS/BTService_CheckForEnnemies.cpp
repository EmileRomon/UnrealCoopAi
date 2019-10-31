// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckForEnnemies.h"
#include "AllyCharacter.h"
#include "AllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTService_CheckForEnnemies::UBTService_CheckForEnnemies()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForEnnemies::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ALeaderController *Controller = Cast<ALeaderController>(OwnerComp.GetAIOwner());
	AAllyCharacter *Char = Cast<AAllyCharacter>(Controller->GetPawn());
	if (Controller && Controller->EnemiesInSigth.Num())
	{
		for (AFPSCharacter *Enemy: Controller->EnemiesInSigth)
		{

			FVector Forward = (Enemy->GetActorLocation() - Char->GetActorLocation());
			if (Enemy && !Enemy->bIsDead && (Char->CurrentWeapon->CanTouch(Enemy, Char->GetActorLocation(), Forward)))
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Controller->EnemyKeyID, Enemy);
				return;
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No enemy %d"), Char->TeamColor));
			Controller->EnemiesInSigth.Remove(Enemy);

		}
		Char->StopFire();
		OwnerComp.GetBlackboardComponent()->ClearValue(Controller->EnemyKeyID);

	}
}