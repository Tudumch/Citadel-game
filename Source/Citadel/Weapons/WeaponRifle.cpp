// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponRifle.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Players/PlayerGround.h"
#include "Components/ImpactFXComponent.h"
#include "Players/PlayerControllerGround.h"

#define OUT

DEFINE_LOG_CATEGORY_STATIC(Log_WeaponRifle, All, All);

AWeaponRifle::AWeaponRifle()
{
    ImpactFXComponent =
        CreateAbstractDefaultSubobject<UImpactFXComponent>(TEXT("ImpactFXComponent"));
}

bool AWeaponRifle::Shoot()
{
    if (!Super::Shoot()) return false;

    FVector TraceStart;
    FVector TraceEnd;
    GetShotStartEndPoints(OUT RifleHitResult, OUT TraceStart, OUT TraceEnd);

    if (APawn* Pawn = Cast<APawn>(RifleHitResult.GetActor()))
    {
        MakeDamageToPawn(Pawn);
    }

    SpawnTraceFX(RifleHitResult.Location);

    return true;
}

void AWeaponRifle::MakeDamageToPawn(APawn* Pawn)
{
    AController* PlayerController = Cast<APawn>(GetOwner())->Controller;

    if (PlayerController)
    {
        UE_LOG(Log_WeaponRifle, VeryVerbose, TEXT("%s has been hit in %s!"),
            *PlayerController->GetName(), *RifleHitResult.BoneName.ToString());

        if (RifleHitResult.BoneName == "head")  // is headshot?
        {
            UGameplayStatics::ApplyDamage(
                Pawn, WeaponDamage * HeadshotMultiplier, PlayerController, this, nullptr);
        }
        else
        {
            UGameplayStatics::ApplyDamage(Pawn, WeaponDamage, PlayerController, this, nullptr);
        }
    }
}

bool AWeaponRifle::ZoomFOV(bool ZoomON)
{
    APlayerControllerGround* PlayerController = Cast<APlayerControllerGround>(GetOwnerController());
    if (!PlayerController) return false;

    const TInterval<float> FOV(50.f, 90.f);

    if (ZoomON)
    {
        PlayerController->PlayerCameraManager->SetFOV(FOV.Min);
    }
    else
    {
        PlayerController->PlayerCameraManager->SetFOV(FOV.Max);
    }

    return true;
}

void AWeaponRifle::SpawnTraceFX(const FVector& EndPoint)
{

    FVector StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);

    const auto TraceFXComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, StartPoint);

    if (TraceFXComponent) TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, EndPoint);

    ImpactFXComponent->PlayImpactFX(RifleHitResult);
}