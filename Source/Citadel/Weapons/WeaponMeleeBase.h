// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMeleeBase.generated.h"

class USphereComponent;

/*
 * Parent class for all melee weapons.
 */
UCLASS()
class CITADEL_API AWeaponMeleeBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponMeleeBase();

    void PlaySwingSound();
    void PlayHitSound();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(EditAnywhere)
    USphereComponent* CollisionComponent;
    UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* SkeletalMesh;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    // ----------
    // FX

    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* SwingSound;
    // Sound playing after successfull hit
    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* HitSound;

private:
    UPROPERTY(EditDefaultsOnly)
    float DamageAmount = 100.f;
};
