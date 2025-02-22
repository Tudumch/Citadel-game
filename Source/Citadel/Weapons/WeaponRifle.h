// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "WeaponRifle.generated.h"

class UImpactFXComponent;
class UNiagaraSystem;
UCLASS()
class CITADEL_API AWeaponRifle : public AWeaponBase
{
    GENERATED_BODY()

public:
    AWeaponRifle();
    virtual bool ZoomFOV(bool ZoomON) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Parameters")
    float HeadshotMultiplier = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    FHitResult RifleHitResult;

    virtual bool Shoot() override;
    virtual void MakeDamageToPawn(APawn* Pawn);

private:
    UPROPERTY(EditAnywhere)
    UImpactFXComponent* ImpactFXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Parameters")
    float WeaponDamage = 10.f;

    void SpawnTraceFX(const FVector& EndPoint);
};
