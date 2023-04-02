// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class CITADEL_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

    // Checks firerate Timers and calls the Shoot function
    virtual bool StartFire();

    // Does some operations with firerate Timers after shot
    virtual void StopFire();

    virtual bool ZoomFOV(bool ZoomON) { return false; };

    void Reload(int32 Amount) { this->AmmoInActiveClip = Amount; };
    int32 GetAmmoInActiveClip() { return this->AmmoInActiveClip; };
    int32 DecreaseAmmoInActiveClip(int32 Amount) { return AmmoInActiveClip -= Amount; };

protected:
    UPROPERTY(EditAnywhere) FName MuzzleSocketName = TEXT("Muzzle_Socket");
    UPROPERTY(EditAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* SkeletalMesh;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    int32 WeaponRange = 2000;
    UPROPERTY(EditAnywhere, Category = "FX")
    UParticleSystem* MuzzleFlashParticle;
    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* ShotSound;

    //
    // FIRERATE

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    float DelayBetweenShots = 0.1f;
    bool bNowFiring = false;  // variable to prevent simultaneous shots. When true, you can't shoot
    FTimerHandle
        DelayBetweenShotsTimerHandle;  // delay between shots when the firing button is pressed
    FTimerHandle
        DelayBetweenMouseClicksTimerHandle;  // delay between repeatedly pressing the shot button

    // Function for use in timers. Calls Shoot function.
    virtual void CallShootFunction();

    //
    // AMMO

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    int32 ClipSize = 30;

    int32 AmmoInActiveClip;

    // Gets player's Pawn and returns Pawn's Controller
    AController* GetOwnerController();

    virtual void BeginPlay() override;
    virtual bool Shoot();
    virtual void SpawnEffects();
    virtual void GetShotStartEndPoints(
        FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint);
    virtual void PrintDebugInfo(FHitResult& HitResult) const;

private:
    // Function for use in timers. Changes the value of the bNowFiring variable to false.
    void Unfire();
};
