// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


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


AHealingWeapon::AHealingWeapon()
: AWeapon()
{
}

void AHealingWeapon::FireHitScan(FVector StartingPosition, FVector ForwardVector)
{
	FHitResult Hit;
	//Weapon spread
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	ForwardVector = FMath::VRandCone(ForwardVector, HalfRad);

	float Damage = BaseDamage;

	FVector End = ((ForwardVector * HitScanRange) + StartingPosition);

	DrawDebugLine(GetWorld(), StartingPosition, End, FColor::Green, false, 1.0f);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActor(MyCharacter);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartingPosition, End, ECC_Visibility, CollisionParameters))
	{
		AFPSCharacter *Char = Cast<AFPSCharacter>(Hit.GetActor());
		UPrimitiveComponent *OtherComp = Hit.GetComponent();
		if (Char)
		{
			TSubclassOf<UDamageType> DamageType;
			if (Char->TeamColor == GetOwnerTeam())
			{
				Damage = -BaseDamage;
			}
			float DamageDeal = UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, GetActorLocation(), Hit, nullptr, this, DamageType);
		}		
		else
		{
			DrawImpact(OtherComp, Hit);
		}
	}
}