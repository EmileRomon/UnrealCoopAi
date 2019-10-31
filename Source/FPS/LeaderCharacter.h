// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllyCharacter.h"
#include "LeaderCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ALeaderCharacter : public AAllyCharacter
{
	GENERATED_BODY()
	
public:
	ALeaderCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float AheadDistance;

};
