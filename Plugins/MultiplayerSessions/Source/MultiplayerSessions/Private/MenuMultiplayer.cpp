// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuMultiplayer.h"

#include "Components/Button.h"

#include "MultiplayerSessionSubsystem.h"

void UMenuMultiplayer::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (HostButton) HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
        MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
}

// Calls when level changes
void UMenuMultiplayer::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    MenuTearDown();
    Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuMultiplayer::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
    NumPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;
    
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    if (GetWorld())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

        if (PlayerController)
        {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }
}

// Removes menu widget and reset input to default value
void UMenuMultiplayer::MenuTearDown()
{
    RemoveFromParent();

    if (GetWorld())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }
}

void UMenuMultiplayer::HostButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString(TEXT("HostButton clicked!")));

    if (MultiplayerSubsystem)
        MultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);

    if (GetWorld())
        GetWorld()->ServerTravel(
            "/Game/Levels/MultiplayerLobbyLevel?listen");  // TODO: remove hardcode
}

void UMenuMultiplayer::JoinButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString(TEXT("JoinButton clicked!")));
}