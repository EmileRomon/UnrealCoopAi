// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/DecalActor.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.2f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}
		ACharacter *Char = Cast<ACharacter>(OtherActor);
		AWeapon *Source = Cast<AWeapon>(GetOwner());
		if (Char && Source)
		{
			//UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorLocation(), Hit, GetOwner()->GetInstigatorController(), this, DamageType);
			float DamageDeal = UGameplayStatics::ApplyPointDamage(OtherActor, Source->BaseDamage, GetActorLocation(), Hit, nullptr, this, DamageType);
		}
		else
		{
			Source->DrawImpact(OtherComp, Hit);
		}
		/*else if(Source->DecalImpact)
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
			ADecalActor *ProjectileImpact = GetWorld()->SpawnActor<ADecalActor>(Hit.Location, Rotation);
			if (ProjectileImpact)
			{
				ProjectileImpact->SetDecalMaterial(DecalImpact);
				ProjectileImpact->SetLifeSpan(ImpactLifeSpan);
				ProjectileImpact->GetDecal()->DecalSize = ImpactSize;
				ProjectileImpact->AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
				ProjectileImpact->SetActorRotation(Rotation);
			}
		}*/
	Destroy();
	}
}

int AFPSProjectile::GetOwnerTeam()
{
	return OwnerTeam;
}

void AFPSProjectile::SetOwnerTeam(int Team)
{
	OwnerTeam = Team;
}
