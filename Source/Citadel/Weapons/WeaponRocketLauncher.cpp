// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponRocketLauncher.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Weapons/WeaponProjectile.h"

bool AWeaponRocketLauncher::Shoot()
{
    if (!Super::Shoot()) return false;

    FVector StartPoint;
    FVector EndPoint;
    GetShotStartEndPoints(HitResult, StartPoint, EndPoint);
    EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndPoint;

    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

    FTransform SpawnLocation(FRotator::ZeroRotator, StartPoint);

    AWeaponProjectile* Projectile =
        GetWorld()->SpawnActorDeferred<AWeaponProjectile>(ProjectileClass, SpawnLocation);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnLocation);
    }

    return true;
}
