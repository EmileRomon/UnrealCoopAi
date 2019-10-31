// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ActorsOfClass.h"
#include "EnvQueryGenerator_FPSCharAround.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UEnvQueryGenerator_FPSCharAround : public UEnvQueryGenerator_ActorsOfClass
{
	GENERATED_BODY()
	

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual bool ShouldAddActor(AActor *Actor, FEnvQueryInstance& QueryInstance) const;
};
