// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponShotgun.h"

bool AWeaponShotgun::StartFire()
{
    if (!Super::StartFire()) return false;

    for (int32 i = 0; i < BulletsPerShot; i++)
        Shoot();

    return true;
}
