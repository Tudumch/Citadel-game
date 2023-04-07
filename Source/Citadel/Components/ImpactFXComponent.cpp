// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ImpactFXComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UImpactFXComponent::UImpactFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UImpactFXComponent::PlayImpactFX()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, GetOwner()->GetActorLocation());
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetOwner()->GetActorLocation());
}

void UImpactFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, Hit.Location);
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.Location);
}
