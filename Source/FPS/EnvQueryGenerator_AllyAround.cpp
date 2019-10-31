// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryGenerator_AllyAround.h"
#include "FPSCharacter.h"

bool UEnvQueryGenerator_AllyAround::ShouldAddActor(AActor *Actor, FEnvQueryInstance& QueryInstance) const
{
	AFPSCharacter *Char = Cast<AFPSCharacter>(Actor);
	AFPSCharacter *Querier = Cast<AFPSCharacter>(QueryInstance.Owner.Get());
	return Char && Querier && Char != Querier && Char->TeamColor == Querier->TeamColor;
}
