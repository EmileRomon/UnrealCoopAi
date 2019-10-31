// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HealingWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHealingWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	AHealingWeapon();

protected:
	virtual void FireHitScan(FVector CharacterLocation, FVector ForwardVector) override;
};
