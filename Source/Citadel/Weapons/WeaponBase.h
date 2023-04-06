// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;
class USkeletalMeshComponent;

/*
Parent class for all ranged weapons.
*/
UCLASS()
class CITADEL_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

    virtual bool ZoomFOV(bool ZoomON) { return false; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    int32 WeaponRange = 10000;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Parameters")
    float BulletSpread = 1.5f;

    // ----------
    // BLUEPRINT COMPONENTS

    UPROPERTY(EditAnywhere)
    FName MuzzleSocketName = TEXT("Muzzle_Socket");
    UPROPERTY(EditAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* SkeletalMesh;

    // ----------
    // FX

    UPROPERTY(EditAnywhere, Category = "FX")
    UParticleSystem* MuzzleFlashParticle;
    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* ShotSound;

    virtual void SpawnEffects();

    // Gets player's Pawn and returns Pawn's Controller
    AController* GetOwnerController();

    virtual void BeginPlay() override;
    virtual void GetShotStartEndPoints(
        FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint);
    virtual void PrintDebugInfo(FHitResult& HitResult) const;

    // ----------
    // FIRERATE
    // ----------
public:
    // Checks firerate Timers and calls the Shoot function
    virtual bool StartFire();
    // Does some operations with firerate Timers after shot
    virtual void StopFire();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    float DelayBetweenShots = 0.1f;
    bool bNowFiring = false;  // variable to prevent simultaneous shots. When true, you can't shoot
    FTimerHandle
        DelayBetweenShotsTimerHandle;  // delay between shots when the firing button is pressed
    FTimerHandle
        DelayBetweenMouseClicksTimerHandle;  // delay between repeatedly pressing the shot button

    virtual bool Shoot();

    // Function for use in timers. Calls Shoot function.
    virtual void CallShootFunction();

private:
    // Function for use in timers. Changes the value of the bNowFiring variable to false.
    void Unfire();

    // ----------
    // AMMO
    // ----------
public:
    // Reloads active weapon.
    void Reload();
    int32 GetAmmoInActiveClip() { return this->AmmoInActiveClip; };
    int32 GetWeaponClipSize() { return ClipSize; }
    int32 DecreaseAmmoInActiveClip(int32 Amount) { return AmmoInActiveClip -= Amount; };
    int32 GetAmmoInInventory() { return this->AmmoInInventory; };
    void AddAmmoInInventory(int32 Amount) { this->AmmoInInventory += Amount; };

    // Tries to take the required amount of ammo from the inventory.
    // Returns how much ammo was taken.
    int32 TakeAmmoFromInventory(int32 Amount);

protected:
private:
    int32 AmmoInActiveClip;  // == ClipSize when BeginPlay()
    int32 AmmoInInventory;   // ammo for this Weapon in Player's inventory

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    int32 ClipSize = 30;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    int32 InitialAmmoInInventory = 0;
};
