// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/PickupWeapon.h"

#include "Weapons/WeaponBase.h"
#include "Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Log_PickupWeapon, All, All);

bool APickupWeapon::GivePickupTo(APawn* Pawn)
{
    Super::GivePickupTo(Pawn);
    return TryToAddWeapon(Pawn);
}

bool APickupWeapon::TryToAddWeapon(APawn* Pawn)
{
    UWeaponComponent* WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return false;

    // Check whether the picking Player already has such a Weapon:
    for (auto Weapon : WeaponComponent->CharacterWeapons)
    {
        if (Weapon->IsA(WeaponClass))
        {
            UE_LOG(Log_PickupWeapon, Verbose,
                TEXT("TryToAddWeapon: %s couldn't pick up weapon because he already has it."),
                *Pawn->GetName());
            return false;
        }
    }

    AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
    WeaponComponent->AddWeaponToPlayer(Weapon);

    return true;
}
