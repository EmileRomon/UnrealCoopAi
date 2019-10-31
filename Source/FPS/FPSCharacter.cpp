// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterSpawn.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// AFPSCharacter

AFPSCharacter::AFPSCharacter()
{

	//Set Health values
	FullHealth = 100.0f;
	RemainingHealth = FullHealth;
	PercentageHealth = 1.0f;
	bCanBeDamaged = true;

	TeamColor = 0;
	bIsDead = false;
	bIsDamaged = false;

	DefaultSpawnLocation = FVector(-300.0f, 100.0f, 200.0f);
	SpawnLocation = DefaultSpawnLocation;
	SpawnRotation = FRotator::ZeroRotator;

	CriticRate = 1.75f;
	HeadBoneName = "head";

	StimuliComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliComp"));

	CanBeHitDelay = 3.0f;
	bCanBeHit = false;

}

void AFPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Allow character to crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	OnDestroyed.AddDynamic(this, &AFPSCharacter::OnDeath);
	FTimerHandle TimeHandleCanBeDamage;
	GetWorld()->GetTimerManager().SetTimer(TimeHandleCanBeDamage, this, &AFPSCharacter::SetCanBeHit, CanBeHitDelay, false);
	
}

void AFPSCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFPSCharacter::Jump()
{
	if (bIsCrouched) {
		UnCrouch();
	}
	Super::Jump();
}

void AFPSCharacter::OnCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

float AFPSCharacter::GetHealth()
{
	return PercentageHealth;
}

void AFPSCharacter::UpdateHealth(float HealthChange)
{
	RemainingHealth += HealthChange;
	RemainingHealth = FMath::Clamp(RemainingHealth, 0.0f, FullHealth);
	PercentageHealth = RemainingHealth / FullHealth;
	if (HealthChange < 0.0f)
	{
		bIsDamaged = true;
	}
	if (RemainingHealth == 0.0f)
	{
		bIsDead = true;
		Destroy();
	}
}

void AFPSCharacter::PlayFireAnimation()
{
	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = BodyMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AFPSCharacter::PlayReloadAnimation()
{
	// try and play the sound if specified
	if (ReloadSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (ReloadAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = BodyMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ReloadAnimation, 1.f);
		}
	}
}
void AFPSCharacter::PlayGetHitAnimation()
{
	// try and play a hit animation if specified
	if (HitAnimation != NULL)
	{
		UAnimInstance* AnimInstance = BodyMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(HitAnimation);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Playing Anim"));
		}
	}
}

FVector AFPSCharacter::GetFireStartingPosition()
{
	return GetActorLocation();
}

FRotator AFPSCharacter::GetFireRotation()
{
	return GetViewRotation();
}

FVector AFPSCharacter::GetFireForwardVector()
{
	return GetActorForwardVector();
}

float AFPSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (bCanBeHit)
	{
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			const FPointDamageEvent &PointEvent = static_cast<const FPointDamageEvent&>(DamageEvent);
			if (PointEvent.HitInfo.BoneName.ToString() == HeadBoneName)
			{
				
				DamageAmount *= CriticRate;
			}
		}
		UpdateHealth(-DamageAmount);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player new health %f "), RemainingHealth));
		PlayGetHitAnimation();
	}
	return DamageAmount;
}

void AFPSCharacter::OnDeath(AActor *Act)
{

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//Search for a spawn if we don't have one set
		if (SpawnerClass && SpawnLocation == DefaultSpawnLocation)
		{
			SearchForSpawn();
		}

		AFPSCharacter *Spawned = World->SpawnActor<AFPSCharacter>(GetClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		//Set the Spawned Character to be the same of us

		if (Spawned)
		{
			Spawned->TeamColor = TeamColor;
			Spawned->SpawnLocation = SpawnLocation;
			Spawned->SpawnRotation = SpawnRotation;
		}

	}
	CurrentWeapon->Destroy();
}

void AFPSCharacter::SearchForSpawn()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerClass, FoundActors);
	for (AActor *act : FoundActors)
	{
		ACharacterSpawn *Spawn = Cast<ACharacterSpawn>(act);
		if (Spawn && Spawn->TeamColor == TeamColor)
		{
			SpawnLocation = Spawn->GetActorLocation();
			SpawnRotation = Spawn->GetActorRotation();
		}
	}
}

void AFPSCharacter::OnFire()
{
	if (!bTryingToFire)
	{
		bTryingToFire = true;
		if (CurrentWeapon)
		{
			CurrentWeapon->Fire();
		}
	}
}

void AFPSCharacter::StopFire()
{
	if (bTryingToFire)
	{
		bTryingToFire = false;
		CurrentWeapon->StopFire();
	}
}

FGenericTeamId AFPSCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(TeamColor);
}

void AFPSCharacter::SetCanBeHit()
{
	bCanBeHit = true;
}
