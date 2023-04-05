// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupBase.h"
#include "PickupWeapon.generated.h"

class AWeaponBase;

/**
 Class for weapon pickups.
 */
UCLASS()
class CITADEL_API APickupWeapon : public APickupBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Settings")
    TSubclassOf<AWeaponBase> WeaponClass;

    virtual bool GivePickupTo(APawn* Pawn) override;

    // Checks if there are already the same weapons in the inventory.
    // Returns true if there is no such weapon.
    bool TryToAddWeapon(APawn* Pawn);
};
