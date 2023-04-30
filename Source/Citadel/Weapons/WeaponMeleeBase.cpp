// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/WeaponMeleeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AWeaponMeleeBase::AWeaponMeleeBase()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComponent;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);  // notify only about colliding with other objects
    CollisionComponent->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Block);  // collision is blocked by all channels

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SkeletalMesh->SetupAttachment(RootComponent);
}

void AWeaponMeleeBase::PlaySwingSound()
{
    if (!SwingSound) return;

    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SwingSound, GetActorLocation());
}

void AWeaponMeleeBase::PlayHitSound()
{

    if (!HitSound) return;
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());
}

void AWeaponMeleeBase::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponMeleeBase::OnOverlapBegin);
}

void AWeaponMeleeBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit!"));

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn || OtherActor == OwnerPawn) return;

    AController* OwnerController = OwnerPawn->Controller;

    UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, OwnerController, OwnerPawn, nullptr);

    PlayHitSound();
}
