// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponComponent.h"

#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponRifle.h"
#include "Weapons/WeaponRocketLauncher.h"
#include "Players/PlayerGround.h"

DEFINE_LOG_CATEGORY_STATIC(Log_WeaponComponent, All, All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    SetupWeapon();
}

void UWeaponComponent::SetupWeapon()
{
    APlayerGround* Player = Cast<APlayerGround>(GetOwner());
    if (!Player) return;

    if (WeaponClasses.Num() == 0)
    {
        UE_LOG(Log_WeaponComponent, Error, TEXT("SetupWeapon: %s: Default Weapons has not set!"),
            *Player->GetName());
        return;
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    for (auto WeaponClass : WeaponClasses)
    {
        AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

        Weapon->AttachToComponent(Player->GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform, ArmoryWeaponSocketName);

        Weapon->SetOwner(Player);
        CharacterWeapons.Add(Weapon);
    }

    ActiveWeapon = CharacterWeapons[ActiveWeaponIdx];
    if (!ActiveWeapon)
    {
        UE_LOG(Log_WeaponComponent, Error, TEXT("SetupWeapon: %s: Can't get active weapon!"),
            *Player->GetName());
        return;
    }

    ActiveWeapon->AttachToComponent(Player->GetMesh(),
        FAttachmentTransformRules::KeepRelativeTransform, ActiveWeaponSocketName);
}

void UWeaponComponent::StartFire()
{
    APlayerGround* Player = Cast<APlayerGround>(GetOwner());

    if (!Player || Player->GetSprinting())
        return;  // Player can't sprint and shoot at the same time

    if (AWeaponRifle* WeaponCasted = Cast<AWeaponRifle>(ActiveWeapon)) WeaponCasted->StartFire();

    if (AWeaponRocketLauncher* WeaponCasted = Cast<AWeaponRocketLauncher>(ActiveWeapon))
        WeaponCasted->StartFire();
}

void UWeaponComponent::StopFire()
{
    APlayerGround* Player = Cast<APlayerGround>(GetOwner());

    if (!Player) return;  // Player can't sprint and shoot at the same time

    if (AWeaponRifle* WeaponCasted = Cast<AWeaponRifle>(ActiveWeapon)) WeaponCasted->StopFire();

    if (AWeaponRocketLauncher* WeaponCasted = Cast<AWeaponRocketLauncher>(ActiveWeapon))
        WeaponCasted->StopFire();
}

void UWeaponComponent::SwitchWeapon()
{
    APlayerGround* Player = Cast<APlayerGround>(GetOwner());
    if (!Player) return;

    ActiveWeapon->AttachToComponent(Player->GetMesh(),
        FAttachmentTransformRules::KeepRelativeTransform, ArmoryWeaponSocketName);

    ActiveWeaponIdx = (ActiveWeaponIdx + 1) % CharacterWeapons.Num();

    ActiveWeapon = CharacterWeapons[ActiveWeaponIdx];

    ActiveWeapon->AttachToComponent(Player->GetMesh(),
        FAttachmentTransformRules::KeepRelativeTransform, ActiveWeaponSocketName);
}

void UWeaponComponent::ToggleZoom(bool ZoomON)
{
    ActiveWeapon->ZoomFOV(ZoomON);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ActiveWeapon = nullptr;

    for (auto Weapon : CharacterWeapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    CharacterWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

int32 FAmmoDataHandler::TakeAmmoRifle(int32 Amount)
{
    RifleAmmo -= Amount;

    if (RifleAmmo < 0)
    {
        Amount -= RifleAmmo;
        RifleAmmo = 0;
    }
    return Amount;
}

int32 FAmmoDataHandler::TakeAmmoRocketLauncher(int32 Amount)
{
    RocketLauncherAmmo -= Amount;

    if (RocketLauncherAmmo < 0)
    {
        Amount -= RocketLauncherAmmo;
        RocketLauncherAmmo = 0;
    }
    return Amount;
}
