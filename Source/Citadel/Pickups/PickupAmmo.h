// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupBase.h"
#include "PickupAmmo.generated.h"

class AWeaponBase;

/**
 Class for ammunition replenishment pickup.
 You can choose a weapon whose ammunition will be replenished.
 */
UCLASS()
class CITADEL_API APickupAmmo : public APickupBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Settings")
    int32 AddAmmoNum = 0;  // how much ammo will be added to the player who picked it up

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Settings")
    TSubclassOf<AWeaponBase> WeaponClass;

    virtual bool GivePickupTo(APawn* Pawn) override;
    bool TryToAddAmmo(APawn* Pawn);
};
