// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dev/DebugSphere.h"
#include "DamageSphere.generated.h"

/**
 * Spherical trigger that deals damage.
 */
UCLASS()
class CITADEL_API ADamageSphere : public ADebugSphere
{
    GENERATED_BODY()

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere)
    float Damage = 0.1f;
};
