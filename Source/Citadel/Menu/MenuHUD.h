// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class CITADEL_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;
};
