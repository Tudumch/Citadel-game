// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "InputActionValue.h"

#include "Weapons/WeaponBase.h"

#include "PlayerGround.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;
class UWeaponComponent;
class UInputMappingContext;
class UInputAction;

/*
Main Pawn class for Players and Bots.
*/
UCLASS()
class CITADEL_API APlayerGround : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerGround(const class FObjectInitializer& ObjectInitializer);

    UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UHealthComponent* HealthComponent;

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION (BlueprintCallable, Category = "TextRender")
        void UpdateHealthRenderText();

    // Changes the base color inside the Pawn's Material
    void SetPlayerColor(FLinearColor Color);

    // ----------
    // PAWN INPUT FUNCTIONS

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

protected:
    UPROPERTY (EditDefaultsOnly, Category = "Materials")
        FName MaterialColorName =
            "BodyColor";  // name of the node that sets the base color of the material for the Pawn

    virtual void BeginPlay() override;

    // Disables Pawn's collision and sets spectating mode for Player Controller
    virtual void OnDeath();

    // Connects EnhancedInputs's MappingContext to Player Controller
    virtual void ConnectMappingContext();

    // -----------------
    // INPUT ACTIONS

    UPROPERTY (EditAnywhere, Category = "Input")
        UInputMappingContext* InputMappingContext;

    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* MoveIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* CrouchIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SprintIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* LookIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* FireIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* AimIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* ReloadIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* HitKnifeIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* ThrowGrenadeIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* ToggleZoomIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToNextIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToPreviousIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex00IA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex01IA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex02IA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex03IA;

private:
    APlayerController* PlayerController;

    UPROPERTY (EditAnywhere)
        UTextRenderComponent* HealthTextRender;
    UPROPERTY (EditAnywhere)
        UWeaponComponent* WeaponComponent;

    void SetupHealthComponent();

    // ----------
    // STANCES
    // ----------
public:
    // ----------
    // GETTERS FOR USE IN ANIMBP

    UFUNCTION (BlueprintPure)
        bool GetCrouching()
        {
            return bCrouching;
        };
    UFUNCTION (BlueprintPure)
        bool GetJogginging()
        {
            return bJogging;
        };
    UFUNCTION (BlueprintPure)
        bool GetSprinting()
        {
            return bSprinting;
        };

protected:
private:
    bool bCrouching = false;
    bool bJogging = true;  // default stance
    bool bSprinting = false;

    // Enum for pass attributes to ToggleStance function
    enum PlayerStances
    {
        Crouching,
        Jogging,
        Sprinting
    };

    // Changes boolean variables that switch stances
    void ToggleStance(PlayerStances Stance);
};
