// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMeleeBase.generated.h"

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
    USkeletalMeshComponent* SkeletalMesh;
    UPROPERTY(EditAnywhere)
    FName BladeStartSocketName = TEXT("BladeStart_Socket");
    UPROPERTY(EditAnywhere)
    FName BladeEndSocketName = TEXT("BladeEnd_Socket");

    // ----------
    // FX

    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* SwingSound;
    // Sound playing after successfull hit
    UPROPERTY(EditAnywhere, Category = "FX")
    USoundBase* HitSound;
};
