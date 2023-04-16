// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponComponent.h"

#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponRifle.h"
#include "Weapons/WeaponRocketLauncher.h"
#include "Weapons/WeaponGrenade.h"
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

    // If Player does not have any initial weapons in array - log error:
    if (WeaponClasses.Num() == 0)
    {
        UE_LOG(Log_WeaponComponent, Error, TEXT("SetupWeapon: %s: default weapons has not set!"),
            *Player->GetName());
        return;
    }

    // Spawning and adding to Player's inventory default weapons:
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    for (auto WeaponClass : WeaponClasses)
    {
        AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
        AddWeaponToPlayer(Weapon);
    }

    SwitchWeaponToIndex(ActiveWeaponIdx);
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

void UWeaponComponent::SwitchWeaponToIndex(int32 Index)
{
    if (Index >= CharacterWeapons.Num() || Index < 0) return;

    APlayerGround* Player = Cast<APlayerGround>(GetOwner());
    if (!Player) return;

    // putting the active weapon in the inventory:
    if (ActiveWeapon)
        ActiveWeapon->AttachToComponent(Player->GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform, ArmoryWeaponSocketName);

    ActiveWeapon = CharacterWeapons[Index];
    ActiveWeapon->AttachToComponent(Player->GetMesh(),
        FAttachmentTransformRules::KeepRelativeTransform, ActiveWeaponSocketName);
}

void UWeaponComponent::SwitchWeaponToNext()
{
    ActiveWeaponIdx = fabs((ActiveWeaponIdx + 1) % CharacterWeapons.Num());
    SwitchWeaponToIndex(ActiveWeaponIdx);
}

void UWeaponComponent::SwitchWeaponToPrevious()
{
    ActiveWeaponIdx = fabs((ActiveWeaponIdx - 1) % CharacterWeapons.Num());
    SwitchWeaponToIndex(ActiveWeaponIdx);
}

void UWeaponComponent::ToggleZoom(bool ZoomON)
{
    ActiveWeapon->ZoomFOV(ZoomON);
}

void UWeaponComponent::ThrowGrenade()
{
    if (GrenadesInInventory <= 0) return;

    FVector ViewLocation;
    FRotator ViewRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation);

    FVector Direction = ViewRotation.Vector();
    FVector Offset = Direction * 100;  // to avoid player collisions during spawn

    FTransform SpawnLocation(FRotator::ZeroRotator, GetOwner()->GetActorLocation() + Offset);

    AWeaponProjectile* Projectile =
        GetWorld()->SpawnActorDeferred<AWeaponProjectile>(GrenadesType, SpawnLocation);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnLocation);  // calls Actor's constructor
    }

    GrenadesInInventory -= 1;
}

void UWeaponComponent::AddWeaponToPlayer(AWeaponBase* Weapon)
{

    APlayerGround* Player = Cast<APlayerGround>(GetOwner());
    if (!Player) return;

    Weapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
        ArmoryWeaponSocketName);

    Weapon->SetOwner(Player);
    CharacterWeapons.Add(Weapon);
}

void UWeaponComponent::ReloadActiveWeapon()
{
    ActiveWeapon->Reload();
}

int32 UWeaponComponent::GetActiveWeaponAmmoInInventory()
{
    return ActiveWeapon->GetAmmoInInventory();
}

int32 UWeaponComponent::GetActiveWeaponAmmoInClip()
{
    return ActiveWeapon->GetAmmoInActiveClip();
}

bool UWeaponComponent::AddGrenadesInInventory(int32 Amount)
{
    if ((GrenadesInInventory + Amount) > MaxGrenadesInInventory) return false;

    GrenadesInInventory = FMath::Clamp(GrenadesInInventory + Amount, 0, MaxGrenadesInInventory);
    return true;
}

bool UWeaponComponent::RemoveGrenadesFromInventory(int32 Amount)
{
    if ((GrenadesInInventory - Amount) < 0) return false;

    GrenadesInInventory = FMath::Clamp(GrenadesInInventory - Amount, 0, MaxGrenadesInInventory);
    return true;
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
