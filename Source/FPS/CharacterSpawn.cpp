// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawn.h"

// Sets default values
ACharacterSpawn::ACharacterSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TeamColor = 0;
}

// Called when the game starts or when spawned
void ACharacterSpawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

