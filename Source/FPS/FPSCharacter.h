// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Weapon.h"
#include "GenericTeamAgentInterface.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class ACharacterSpawn;

UCLASS(config=Game)
class AFPSCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AFPSCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);

	/** Character jump*/
	void Jump();

	/** Character crouch/uncrouch*/
	void OnCrouch();

	/** When character died */
	UFUNCTION()
	void OnDeath(AActor *Act);

	/*Search for a spawn*/
	void SearchForSpawn();

	bool bTryingToFire;

	virtual FGenericTeamId GetGenericTeamId() const override;

	void SetCanBeHit();

	bool bCanBeHit;

public:

	/** Pawn body mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* BodyMesh;

	/** Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	AWeapon *CurrentWeapon;

	/** Weapon Class*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<AWeapon> WeaponClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Sound to play each time we reload */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ReloadSound;

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;

	/** Maximum health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float FullHealth;

	/** Remaining health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float RemainingHealth;

	/** Remaining health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float PercentageHealth;
	
	/**true is character health is 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	bool bIsDead;

	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = Health)
	void UpdateHealth(float HealthChange);

	/** Fires a projectile. */
	void OnFire();

	/** Stop to fire with weapon */
	void StopFire();

	/*Class of the Actor where we want to respawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<ACharacterSpawn> SpawnerClass;

	/*Default Location where we want to respawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	FVector DefaultSpawnLocation;

	/*Time where the character can't be damaged*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	float CanBeHitDelay;

	/*Location where we want to respawn*/
	FVector SpawnLocation;

	/*Rotator where we want to respawn*/
	FRotator SpawnRotation;

	/** AnimMontage to play each time we get hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* HitAnimation;

	/**Character team*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	int TeamColor;

	/**Damage multiplier when critic*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float CriticRate;

	/**Name of the head bone*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString HeadBoneName;

	bool bIsDamaged;

	/**Play Character fire animation and sound*/
	void PlayFireAnimation();

	/**Play Character reload animation and sound*/
	void PlayReloadAnimation();

	/**Play Characterhit reaction animation and sound*/
	void PlayGetHitAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAIPerceptionStimuliSourceComponent *StimuliComp;

	virtual FVector GetFireStartingPosition();
	virtual FRotator GetFireRotation();
	virtual FVector GetFireForwardVector();

};

