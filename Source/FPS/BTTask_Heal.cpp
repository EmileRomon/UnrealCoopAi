// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Heal.h"
#include "AllyCharacter.h"
#include "MedicController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMedicController *Controller = Cast<AMedicController>(OwnerComp.GetAIOwner());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Heal")));
	if (Controller)
	{
		AFPSCharacter *Ally = Cast<AFPSCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(Controller->AllyHealKeyID));
		AAllyCharacter *Char = Cast<AAllyCharacter>(Controller->GetPawn());
		if (Char)
		{
			if (Ally && !Ally->bIsDead && Ally->GetHealth() < 1.0f)
			{
				FVector Forward = (Ally->GetActorLocation() - Char->GetActorLocation());
				if (Char->CurrentWeapon->CanTouch(Ally, Char->GetActorLocation(), Forward))
				{
					Char->FireToDirection(Forward);

					Char->StopFire();
					//OwnerComp.GetBlackboardComponent()->ClearValue(Controller->AllyHealKeyID);

					return EBTNodeResult::Succeeded;
				}
			}
			Char->StopFire();
			OwnerComp.GetBlackboardComponent()->ClearValue(Controller->AllyHealKeyID);
		}
	}
	return EBTNodeResult::Failed;
}
