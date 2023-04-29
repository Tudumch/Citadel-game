// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;
class AWeaponGrenade;
class AWeaponMeleeBase;

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
    void HitKnife();

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
    TSubclassOf<AWeaponMeleeBase> WeaponMeleeType;

    AWeaponMeleeBase* MeleeWeapon;

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
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* MeleeHitAnimation;

    // If true - Player can't do some actions like firing, reloading etc..
    bool bBlockingAnimationInProgress = false;

    // Setups weapons in inventory after Player's respawn.
    void SetupWeapon();

    void PlayAnimMontage(UAnimMontage* AnimMontage);

    // Setups binding to several AnimNotifies.
    void SetupAllAnimNotifies();

    // Finds AnimationFinishedAnimNotify from all AnimMontage's Notifies and binds calling function
    // to it.
    void SubscribeToAnimationFinishedNotify(UAnimMontage* AnimMontage);
    // Finds AnimationStartedAnimNotify from all AnimMontage's Notifies and binds calling function
    // to it.
    void SubscribeToAnimationStartedNotify(UAnimMontage* AnimMontage);

    // Switches bBlockingAnimationInProgress to false state.
    void OnAnimationFinished(USkeletalMeshComponent* SkeletalMesh);
    // Hides melee weapon and unhides default weapon after HitKnife animation finishes to play.
    void OnHitKnifeAnimationFinished(USkeletalMeshComponent* SkeletalMesh);

    // Switches bBlockingAnimationInProgress to true state and calls StopFire function.
    void OnAnimationStarted(USkeletalMeshComponent* SkeletalMesh);
};
