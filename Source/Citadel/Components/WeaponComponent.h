// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;

// Stores and changes info about all types of ammo, which player has in inventory.
USTRUCT(BlueprintType)
struct FAmmoDataHandler
{
    GENERATED_BODY()

public:
    int32 GetAmmoRifle() { return this->RifleAmmo; };
    void AddAmmoRifle(int32 Amount) { this->RifleAmmo += Amount; };
    int32 TakeAmmoRifle(int32 Amount);

    int32 GetAmmoRocketLauncher() { return this->RocketLauncherAmmo; };
    void AddAmmoRocketLauncher(int32 Amount) { this->RocketLauncherAmmo += Amount; };
    int32 TakeAmmoRocketLauncher(int32 Amount);

private:
    int32 RifleAmmo = 120;
    int32 RocketLauncherAmmo = 4;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CITADEL_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    FAmmoDataHandler AmmoDataHandler;

    void StartFire();
    void StopFire();
    void SwitchWeapon();
    void ToggleZoom(bool ZoomON);

    UFUNCTION()
    void ReloadActiveWeapon();
    UFUNCTION(BlueprintPure)
    int32 GetActiveWeaponTotalAmmo();
    UFUNCTION(BlueprintPure)
    int32 GetActiveWeaponAmmoInClip();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

    virtual void BeginPlay() override;

    // Removes Weapon mesh from scene after Player's death
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    int32 ActiveWeaponIdx = 0;  // index of the current weapon in hands

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ActiveWeaponSocketName = TEXT("WeaponSocket_r");
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ArmoryWeaponSocketName = TEXT("ArmorySocket");

    TArray<AWeaponBase*> CharacterWeapons;  // weapon in inventory
    AWeaponBase* ActiveWeapon;              // weapon in hands

    void SetupWeapon();  // TODO: need to decompose!
};
