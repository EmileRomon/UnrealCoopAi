// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSCharacter.h"
#include "FPSGameMode.generated.h"

UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUknown
};


UCLASS(minimalapi)
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	AFPSCharacter* MainCharacter;

	UFUNCTION(BlueprintPure, category = Health)
	EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState NewState);

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);
};



