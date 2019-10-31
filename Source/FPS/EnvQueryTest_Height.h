// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_Height.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UEnvQueryTest_Height : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionDetails() const override;
};
