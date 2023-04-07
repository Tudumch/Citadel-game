// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponProjectile.h"
#include "WeaponGrenade.generated.h"

/**
 * Inherited from the WeaponProjectile class.
 */
UCLASS()
class CITADEL_API AWeaponGrenade : public AWeaponProjectile
{
    GENERATED_BODY()

public:
    // Makes radial damage
    virtual void Explode();

protected:
    // Plays the sound of a grenade hitting the ground
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void PlayExplosionFX();

    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float ExplosionDelay = 3.f;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UParticleSystem* ExplosionParticle;

    UPROPERTY(EditAnywhere, Category = "VFX")
    USoundBase* ExplosionSound;

    FTimerHandle ExplosionDelayTimerHandle;
};
