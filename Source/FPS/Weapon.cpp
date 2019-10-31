// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "MainCharacter.h"
#include "AllyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/DecalActor.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "TimerManager.h"


#include "DrawDebugHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	//GunMeshComp->bCastDynamicShadow = false;
	//GunMeshComp->CastShadow = false;
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	GunMeshComp->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMeshComp);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	//Set the starter load
	FullLoader = 30;
	CountLoader = FullLoader;

	bIsHitScan = true;
	HitScanRange = 4000.0f;

	BaseDamage = 20.0f;

	ImpactLifeSpan = 3.0f;
	ImpactSize = FVector(7.0, 7.0, 7.0);

	BulletSpreadMax = 3.0f;
	BulletSpreadRate = 0.5f;
	BulletSpread = 0.1f;

	LastFire = 0.0f;

	DelayBetweenShots = 0.2f;

}

int AWeapon::GetOwnerTeam()
{
	return MyCharacter->TeamColor;
}

void AWeapon::DrawImpact(UPrimitiveComponent * Target, const FHitResult& HitResult)
{
	if (DecalImpact && Target)
	{
		/*if (OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulseAtLocation(ForwardVector * 3000.0f, GetActorLocation());
		}*/
		FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);
		ADecalActor *ProjectileImpact = GetWorld()->SpawnActor<ADecalActor>(HitResult.Location, Rotation);
		if (ProjectileImpact)
		{
			ProjectileImpact->SetDecalMaterial(DecalImpact);
			ProjectileImpact->SetLifeSpan(ImpactLifeSpan);
			ProjectileImpact->GetDecal()->DecalSize = ImpactSize;
			ProjectileImpact->AttachToComponent(Target, FAttachmentTransformRules::KeepWorldTransform);
			ProjectileImpact->SetActorRotation(Rotation);
		}
	}

}

bool AWeapon::CanTouch(AActor * Target, FVector StartingPosition, FVector ForwardVector)
{
	FHitResult Hit;
	FVector End = ((ForwardVector * HitScanRange) + StartingPosition);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActor(MyCharacter);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartingPosition, End, ECC_Visibility, CollisionParameters) && CanFire())
	{
		return Hit.GetActor() == Target;
	}

	return false;
}

void AWeapon::SetOwnerCharacter(AFPSCharacter * Character)
{
	MyCharacter = Character;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

bool AWeapon::CanFire()
{
	return !bTryingToFire && MyCharacter;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AWeapon::GetLoader()
{
	return CountLoader;
}

int AWeapon::GetFullLoader()
{
	return FullLoader;
}

void AWeapon::Fire()
{
	if (CanFire())
	{
		bTryingToFire = true;
		float InGameTime = GetWorld()->GetTimeSeconds();
		if (LastFire + DelayBetweenShots > InGameTime)
		{
			//Delay shot
			GetWorld()->GetTimerManager().SetTimer(TimeHandleFiring, this, &AWeapon::StartFiring, LastFire + DelayBetweenShots - InGameTime, false);
		}
		else
		{
			StartFiring();
		}
	}
}

void AWeapon::StartFiring()
{
	if (CountLoader > 0)
	{		
		CountLoader--;
		//	FVector StartingPosition = MuzzleLocation->GetComponentLocation();
		FVector StartingPosition = MyCharacter->GetFireStartingPosition();

		FRotator CharacterRotation = MyCharacter->GetFireRotation();
		FVector ForwardVector = MyCharacter->GetFireForwardVector();

		if (BulletSpread < BulletSpreadMax)
		{
			BulletSpread += BulletSpreadRate;
			if (BulletSpread > BulletSpreadMax)
			{
				BulletSpread = BulletSpreadMax;
			}
		}

		if (bIsHitScan)
		{
			FireHitScan(StartingPosition, ForwardVector);
		}
		else
		{
			FireProjectile(StartingPosition, CharacterRotation);
		}

		MyCharacter->PlayFireAnimation();
	}
	else
	{
		Reload();
	}

	if (bTryingToFire)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeHandleFiring, this, &AWeapon::StartFiring, DelayBetweenShots, false);
	}

	LastFire = GetWorld()->GetTimeSeconds();
}

void AWeapon::StopFire()
{
	bTryingToFire = false;
	BulletSpread = 0.1f;
	GetWorldTimerManager().ClearTimer(TimeHandleFiring);
}

void AWeapon::Reload()
{
	if (CountLoader < FullLoader)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloading"));
		CountLoader = FullLoader;
	}
}

void AWeapon::FireHitScan(FVector StartingPosition, FVector ForwardVector)
{
	FHitResult Hit;
	//Weapon spread
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	ForwardVector = FMath::VRandCone(ForwardVector, HalfRad);

	FVector End = ((ForwardVector * HitScanRange) + StartingPosition);

	DrawDebugLine(GetWorld(), StartingPosition, End, FColor::Green, false, 1.0f);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActor(MyCharacter);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartingPosition, End, ECC_Visibility, CollisionParameters))
	{
		AFPSCharacter *Char = Cast<AFPSCharacter>(Hit.GetActor());
		UPrimitiveComponent *OtherComp = Hit.GetComponent();
		if (Char && Char->TeamColor != GetOwnerTeam())
		{
			TSubclassOf<UDamageType> DamageType;
			//UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorLocation(), Hit, GetOwner()->GetInstigatorController(), this, DamageType);
			float DamageDeal = UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BaseDamage, GetActorLocation(), Hit, nullptr, this, DamageType);
		}
		else
		{
			DrawImpact(OtherComp, Hit);
		}
	}
}

void AWeapon::FireProjectile(FVector CharacterLocation, FRotator CharacterRotation)
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = CharacterRotation;
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() :CharacterLocation) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			// spawn the projectile at the muzzle
			AFPSProjectile *proj = World->SpawnActor<AFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (proj)
			{
				proj->SetOwner(this);
				proj->SetOwnerTeam(GetOwnerTeam());
			}
		}
	}
}