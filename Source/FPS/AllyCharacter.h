// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "AllyCharacter.generated.h"

UCLASS()
class FPS_API AAllyCharacter : public AFPSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAllyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Distance in witch we are "close" to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptanceDistanceToPlayer;

	AFPSCharacter *TeamLeader;

	void InitTeamLeader();

	FVector FireDirection;


public:	

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree *BotBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TSubclassOf<AFPSCharacter> LeaderClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TSubclassOf<AFPSCharacter> AlliesClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetAcceptanceDistanceToPlayer();

	void FireToDirection(FVector ForwardVector);

	AFPSCharacter * GetTeamLeader();

	virtual FVector GetFireStartingPosition() override;
	virtual FVector GetFireForwardVector() override;

};
