// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
	: Super()
{
	//PrimaryActorTick.bStartWithTickEnabled = true;
	//PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/Character/MainCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);
	MainCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AFPSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSGameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

EGamePlayState AFPSGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AFPSGameMode::HandleNewState(EGamePlayState NewState)
{
	if (NewState == EGamePlayState::EGameOver)
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


