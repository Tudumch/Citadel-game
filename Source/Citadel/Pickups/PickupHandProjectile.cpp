// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/PickupHandProjectile.h"

#include "Weapons/WeaponProjectile.h"
#include "Weapons/WeaponGrenade.h"
#include "Components/WeaponComponent.h"

bool APickupHandProjectile::GivePickupTo(APawn* Pawn)
{
    return TryToAddAmmo(Pawn);
}

bool APickupHandProjectile::TryToAddAmmo(APawn* Pawn)
{
    UWeaponComponent* WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return false;

    if (Cast<AWeaponGrenade>(ProjectileClass->GetDefaultObject()))
    {
        return WeaponComponent->AddGrenadesInInventory(AddAmmoNum);
    }

    return false;
}
