// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvQueryGenerator_FPSCharAround.h"
#include "EnvQueryGenerator_AllyAround.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UEnvQueryGenerator_AllyAround : public UEnvQueryGenerator_FPSCharAround
{
	GENERATED_BODY()
	
		virtual bool ShouldAddActor(AActor *Actor, FEnvQueryInstance& QueryInstance) const;

};
