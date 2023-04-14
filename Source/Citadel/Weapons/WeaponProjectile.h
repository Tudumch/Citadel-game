// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UImpactFXComponent;

/*
Parent Class for all projectiles.
*/
UCLASS()
class CITADEL_API AWeaponProjectile : public AActor
{
    GENERATED_BODY()

public:
    AWeaponProjectile();

    void SetShotDirection(FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(EditAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere)
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere)
    UImpactFXComponent* ImpactFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.f;

    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    FVector ShotDirection;

    void AlignForwardVectorWithOwner();
};
