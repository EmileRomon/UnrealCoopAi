// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CkeckForAllies.h"
#include "AllyCharacter.h"
#include "MedicController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTService_CkeckForAllies::UBTService_CkeckForAllies()
{
	bCreateNodeInstance = true;
}

void UBTService_CkeckForAllies::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AMedicController *Controller = Cast<AMedicController>(OwnerComp.GetAIOwner());
	AAllyCharacter *Char = Cast<AAllyCharacter>(Controller->GetPawn());
	if (Controller && Controller->AlliesInSigth.Num())
	{
		for (AFPSCharacter *Ally : Controller->AlliesInSigth)
		{

			FVector Forward = (Ally->GetActorLocation() - Char->GetActorLocation());
			if (Ally && !Ally->bIsDead && (Char->CurrentWeapon->CanTouch(Ally, Char->GetActorLocation(), Forward)) && Ally->RemainingHealth != Ally->FullHealth)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Controller->AllyHealKeyID, Ally);
				return;
			}
			Controller->AlliesInSigth.Remove(Ally);

		}
		Char->StopFire();
		OwnerComp.GetBlackboardComponent()->ClearValue(Controller->AllyHealKeyID);

	}
}
