// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupBase.h"
#include "PickupHandProjectile.generated.h"

class AWeaponProjectile;

/**
 * Class for hand-held projectiles (grenades, etc.).
 */
UCLASS()
class CITADEL_API APickupHandProjectile : public APickupBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Settings")
    int32 AddAmmoNum = 0;  // how much ammo will be added to the player who picked it up

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Settings")
    TSubclassOf<AWeaponProjectile> ProjectileClass;

    virtual bool GivePickupTo(APawn* Pawn) override;
    bool TryToAddAmmo(APawn* Pawn);
};
