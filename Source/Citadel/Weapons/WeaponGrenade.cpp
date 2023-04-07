// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponGrenade.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ImpactFXComponent.h"

void AWeaponGrenade::BeginPlay()
{
    Super::BeginPlay();

    // Set explosion Timer
    GetWorldTimerManager().SetTimer(
        ExplosionDelayTimerHandle, this, &ThisClass::Explode, ExplosionDelay, true);
}

void AWeaponGrenade::Explode()
{
    ProjectileMovementComponent->StopMovementImmediately();

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    AController* OwnerController = OwnerPawn->Controller;

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius,
        UDamageType::StaticClass(), {}, this, OwnerController, false);

    PlayExplosionFX();
    Destroy();
}

void AWeaponGrenade::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ImpactFXComponent->PlayImpactFX();
}

void AWeaponGrenade::PlayExplosionFX()
{
    UGameplayStatics::SpawnEmitterAtLocation(
        this, ExplosionParticle, GetOwner()->GetActorLocation());
    UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetOwner()->GetActorLocation());
}
