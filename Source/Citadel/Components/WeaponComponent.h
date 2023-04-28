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
    // Index of the current weapon in hands
    int32 ActiveWeaponIdx = -1;  // initial value is -1 to switch weapons correctly
                                 // when the player spawns (see SwitchWeaponToIndex logic)

public:
    UWeaponComponent();

    // Weapons in inventory
    TArray<AWeaponBase*> CharacterWeapons;
    // Weapon in hands
    AWeaponBase* ActiveWeapon;

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
    // Weapons that Player has at respawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

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

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* WeaponEquipAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* WeaponReloadAnimation;

    // If true - Player can't do some actions like firing, reloading etc..
    bool bBlockingAnimationInProgress = false;

    // Setups weapons in inventory after Player's respawn.
    void SetupWeapon();

    void PlayAnimMontage(UAnimMontage* AnimMontage);

    // Binds AnimNotifies callbacks.
    void SetupAllAnimNotifies();
    void SetupEquipAnimNotify();
    void SetupReloadAnimNotify();

    void OnAnimationFinished(USkeletalMeshComponent* SkeletalMesh);
};
