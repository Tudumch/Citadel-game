// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponRifle.h"
#include "WeaponShotgun.generated.h"

/**
 * The class inherits from WeaponRifle.
 */
UCLASS()
class CITADEL_API AWeaponShotgun : public AWeaponRifle
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Parameters")
    int32 BulletsPerShot = 5;  // how many bullets are fired in one shot

    virtual bool StartFire() override;
};
