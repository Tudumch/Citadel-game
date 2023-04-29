// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponMeleeBase.h"

#include "Kismet/GameplayStatics.h"

AWeaponMeleeBase::AWeaponMeleeBase()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComponent;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SkeletalMesh->SetupAttachment(SceneComponent);
}

void AWeaponMeleeBase::PlaySwingSound()
{
    if (!SwingSound) return;

    UGameplayStatics::SpawnSoundAttached(SwingSound, SkeletalMesh, BladeEndSocketName);
}

void AWeaponMeleeBase::PlayHitSound()
{

    if (!HitSound) return;
    UGameplayStatics::SpawnSoundAttached(HitSound, SkeletalMesh, BladeEndSocketName);
}

void AWeaponMeleeBase::BeginPlay()
{
    Super::BeginPlay();
}
