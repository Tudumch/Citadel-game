// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/PlayerControllerGround.h"

#include "Blueprint/UserWidget.h"

#include "Components/RespawnComponent.h"


APlayerControllerGround::APlayerControllerGround()
{
    RespawnComponent = CreateDefaultSubobject<URespawnComponent>(
                TEXT("RespawnComponent"));
}

void APlayerControllerGround::BeginPlay()
{
    Super::BeginPlay();
}