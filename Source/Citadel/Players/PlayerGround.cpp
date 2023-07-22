// Fill out your copyright notice in the Description page of Project Settings.

#include "Citadel/Players/PlayerGround.h"

#include "Components/CapsuleComponent.h"
// #include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CustomCharacterMovementComponent.h"
#include "Weapons/WeaponRifle.h"

APlayerGround::APlayerGround(const class FObjectInitializer& ObjectInitializer)
    // overriding CharacterMovementComponent:
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
          ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    HealthTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthRenderer"));
    HealthTextRender->SetupAttachment(RootComponent);

    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void APlayerGround::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = GetWorld()->GetFirstPlayerController();
    ConnectMappingContext();

    SetupHealthComponent();
}

// Called every frame
void APlayerGround::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// -------------------------------------------------------------------

void APlayerGround::SetupHealthComponent()
{
    UpdateHealthRenderText();
    HealthComponent->OnDeath.AddUObject(
        this, &APlayerGround::OnDeath);  // Subscribe on C++ only delegate
    HealthComponent->OnDamage.AddDynamic(this,
        &APlayerGround::UpdateHealthRenderText);  // Subscribe on universal
                                                  // delegate
}

// --------------------------------------------------
// INPUTS
// --------------------------------------------------
void APlayerGround::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
            CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(
            MoveIA, ETriggerEvent::Triggered, this, &ThisClass::Move);
        EnhancedInputComponent->BindAction(
            LookIA, ETriggerEvent::Triggered, this, &ThisClass::Look);

        // ----------
        // STANCE TOGGLING:

        DECLARE_DELEGATE_OneParam(FToggleStanceInputParams,
            PlayerStances);  // to pass an attribute to a function by reference below

        EnhancedInputComponent->BindAction(CrouchIA, ETriggerEvent::Triggered, this,
            &ThisClass::ToggleStance, PlayerStances::Crouching);
        EnhancedInputComponent->BindAction(SprintIA, ETriggerEvent::Started, this,
            &ThisClass::ToggleStance, PlayerStances::Sprinting);
        EnhancedInputComponent->BindAction(SprintIA, ETriggerEvent::Completed, this,
            &ThisClass::ToggleStance, PlayerStances::Jogging);

        // ---------
        // WEAPONS:

        EnhancedInputComponent->BindAction(
            FireIA, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(
            FireIA, ETriggerEvent::Completed, WeaponComponent, &UWeaponComponent::StopFire);
        EnhancedInputComponent->BindAction(ReloadIA, ETriggerEvent::Triggered, WeaponComponent,
            &UWeaponComponent::ReloadActiveWeapon);
        EnhancedInputComponent->BindAction(ThrowGrenadeIA, ETriggerEvent::Started, WeaponComponent,
            &UWeaponComponent::ThrowGrenade);
        EnhancedInputComponent->BindAction(
            HitKnifeIA, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::HitKnife);

        // ----------
        // SWITCH WEAPON
        EnhancedInputComponent->BindAction(SwitchWeaponToNextIA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToNext);
        EnhancedInputComponent->BindAction(SwitchWeaponToPreviousIA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToPrevious);

        DECLARE_DELEGATE_OneParam(FSwitchWeaponInputParams,
            int32);  // to pass an attribute to a function by reference below
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex00IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 0);
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex01IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 1);
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex02IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 2);
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex03IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 3);

        // ----------
        // ZOOM:
        DECLARE_DELEGATE_OneParam(
            FZoomInputParams, bool);  // to pass an attribute to a function by reference below
        EnhancedInputComponent->BindAction(ToggleZoomIA, ETriggerEvent::Started, WeaponComponent,
            &UWeaponComponent::ToggleZoom, true);
        EnhancedInputComponent->BindAction(ToggleZoomIA, ETriggerEvent::Completed, WeaponComponent,
            &UWeaponComponent::ToggleZoom, false);
    }
}

void APlayerGround::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.X);

    if (bSprinting) return;  // player can't strafe while sprinting
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void APlayerGround::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();
    if (GetController())
    {
        AddControllerYawInput(LookAxisValue.X);
        AddControllerPitchInput(LookAxisValue.Y);
    }
}

void APlayerGround::ToggleStance(PlayerStances Stance)
{
    if (Stance == PlayerStances::Crouching)
    {
        if (bCrouching)
        {
            bCrouching = false;
            bJogging = true;
            bSprinting = false;
        }
        else
        {
            bCrouching = true;
            bJogging = false;
            bSprinting = false;
        }
    }

    if (Stance == PlayerStances::Sprinting)
    {
        if (bSprinting)
        {
            bCrouching = false;
            bJogging = true;
            bSprinting = false;
        }
        else
        {
            bCrouching = false;
            bJogging = false;
            bSprinting = true;
        }
    }

    if (Stance == PlayerStances::Jogging)
    {
        bCrouching = false;
        bJogging = true;
        bSprinting = false;
    }
}

// --------------------------------------------------

void APlayerGround::UpdateHealthRenderText()
{
    HealthTextRender->SetText(FText::AsNumber(HealthComponent->GetHealth()));
}

void APlayerGround::OnDeath()
{
    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }

    if (PlayerController) GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (Controller) Controller->ChangeState(NAME_Spectating);

    SetLifeSpan(5.f);
}

void APlayerGround::ConnectMappingContext()
{
    if (PlayerController)
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

void APlayerGround::SetPlayerColor(FLinearColor Color)
{
    auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}
