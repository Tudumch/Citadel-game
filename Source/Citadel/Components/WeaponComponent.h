// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;
class AWeaponGrenade;

/*
Class for interaction between the player and his weapon.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CITADEL_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

private:
    int32 ActiveWeaponIdx = 0;  // index of the current weapon in hands

public:
    UWeaponComponent();

    TArray<AWeaponBase*> CharacterWeapons;  // weapons in inventory
    AWeaponBase* ActiveWeapon;              // weapon in hands

    void StartFire();
    void StopFire();
    void SwitchWeaponToIndex(int32 Index);
    void SwitchWeaponToNext();
    void SwitchWeaponToPrevious();
    void ToggleZoom(bool ZoomON);
    void ThrowGrenade();

    // Adds Weapon to Player's inventory.
    void AddWeaponToPlayer(AWeaponBase* Weapon);

    // ----------
    // WEAPON AMMO

    UFUNCTION()
    void ReloadActiveWeapon();
    UFUNCTION(BlueprintPure)
    int32 GetActiveWeaponAmmoInInventory();
    UFUNCTION(BlueprintPure)
    int32 GetActiveWeaponAmmoInClip();

    // ----------
    // GRENADES

    UFUNCTION(BlueprintPure)
    int32 GetGrenadesInInventory() { return GrenadesInInventory; };
    UFUNCTION()
    bool AddGrenadesInInventory(int32 Amount);
    UFUNCTION()
    bool RemoveGrenadesFromInventory(int32 Amount);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeaponBase>>
        WeaponClasses;  // what weapons does the Player have after respawn

    virtual void BeginPlay() override;

    // Removes Weapon mesh from scene after Player's death
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeaponGrenade> GrenadesType;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 GrenadesInInventory = 0;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 MaxGrenadesInInventory = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ActiveWeaponSocketName = TEXT("WeaponSocket_r");
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ArmoryWeaponSocketName = TEXT("ArmorySocket");

    // Setups weapons in inventory after Player's respawn.
    void SetupWeapon();
};
