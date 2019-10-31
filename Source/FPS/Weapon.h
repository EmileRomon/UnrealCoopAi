// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	/** Gun mesh*/
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* GunMeshComp;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

	/** Characters weapon is a Hitscan */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		bool bIsHitScan;

	/** Range of the Hitscan shot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float HitScanRange;

	/** Maximum number in the weapon loader*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loader)
		int FullLoader;

	/** Remaining shots*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loader)
		int CountLoader;

	UFUNCTION(BlueprintPure, Category = Loader)
		int GetLoader();

	UFUNCTION(BlueprintPure, Category = Loader)
	int GetFullLoader();

	void Fire();

	void StopFire();

	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impact)
	UMaterialInterface* DecalImpact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impact)
	float ImpactLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impact)
	FVector ImpactSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spread)
		float BulletSpreadMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spread)
		float BulletSpreadRate;

	float BulletSpread;

	void SetOwnerCharacter(class AFPSCharacter *Character);

	int GetOwnerTeam();

	FTimerHandle TimeHandleFiring;

	void DrawImpact(class UPrimitiveComponent *Target, const FHitResult& Hit);

	bool CanTouch(AActor *Target, FVector StartingPosition, FVector ForwardVector);

protected:

	class AFPSCharacter *MyCharacter;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float LastFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float DelayBetweenShots;

	bool bTryingToFire;

	bool CanFire();

	void StartFiring();

	virtual void FireHitScan(FVector CharacterLocation, FVector ForwardVector);

	void FireProjectile(FVector CharacterLocation, FRotator CharacterRotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
