// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyCharacter.h"
#include "FPSProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "EngineUtils.h"

class USkeletalMeshComponent;

// Sets default values
AAllyCharacter::AAllyCharacter()
: AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AcceptanceDistanceToPlayer = 600.0f;
	
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	BodyMesh->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	TeamColor = 1;
}

// Called when the game starts or when spawned
void AAllyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn a default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, GetActorRotation(), SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->SetOwnerCharacter(this);
		CurrentWeapon->AttachToComponent(BodyMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponSocket"));
	}
	InitTeamLeader();
}

void AAllyCharacter::InitTeamLeader()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), LeaderClass, FoundActors);
	for (AActor *act : FoundActors)
	{
		AFPSCharacter *Found = Cast<AFPSCharacter>(act);
		if (Found && Found->TeamColor == TeamColor)
		{
			TeamLeader = Found;
		}
	}
}

// Called every frame
void AAllyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAllyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAllyCharacter::GetAcceptanceDistanceToPlayer()
{
	return AcceptanceDistanceToPlayer;
}

void AAllyCharacter::FireToDirection(FVector ForwardVector)
{
	FireDirection = ForwardVector;
	OnFire();
}

AFPSCharacter * AAllyCharacter::GetTeamLeader()
{
	if ((!TeamLeader || TeamLeader->bIsDead) && LeaderClass)
	{
		InitTeamLeader();
	}
	return TeamLeader;
}

FVector AAllyCharacter::GetFireStartingPosition()
{
	FVector StartingPosition = GetActorLocation();
	StartingPosition.Z += 50.0f;
	return StartingPosition;
}

FVector AAllyCharacter::GetFireForwardVector()
{
	return FireDirection;
}
