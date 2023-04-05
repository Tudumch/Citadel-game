// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/PickupAmmo.h"

#include "Weapons/WeaponBase.h"
#include "Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Log_PickupAmmo, All, All);

bool APickupAmmo::GivePickupTo(APawn* Pawn)
{
    return TryToAddAmmo(Pawn);
}

bool APickupAmmo::TryToAddAmmo(APawn* Pawn)
{
    UWeaponComponent* WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return false;

    for (auto Weapon : WeaponComponent->CharacterWeapons)
    {
        if (Weapon->IsA(WeaponClass))
        {
            Weapon->AddAmmoInInventory(AddAmmoNum);
            return true;
        }
    }

    UE_LOG(Log_PickupAmmo, Verbose,
        TEXT("TryToAddAmmo: %s couldn't pick up ammo because he doesn't have a suitable weapon in "
             "the inventory"),
        *Pawn->GetName());
    return false;
}
