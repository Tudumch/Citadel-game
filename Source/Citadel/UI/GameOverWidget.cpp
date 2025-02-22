// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GameOverWidget.h"

#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "Components/PlayerStateBase.h"
#include "CitadelGameModeBase.h"
#include "UI/StatisticWidget.h"

bool UGameOverWidget::Initialize()
{
    Super::Initialize();
    ACitadelGameModeBase* GameMode = Cast<ACitadelGameModeBase>(GetWorld()->GetAuthGameMode());

    if (GameMode)
    {
        GameMode->OnMatchStateChanged.AddUObject(this, &UGameOverWidget::OnMatchStateChanged);
    }

    if (ResetButton) ResetButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnResetLevel);

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickMainMenu);

    return true;
}

void UGameOverWidget::OnMatchStateChanged(CitadelMatchState State)
{
    if (State == CitadelMatchState::GameOver)
    {
        UpdatePlayerStatistic();
    }
}

void UGameOverWidget::UpdatePlayerStatistic()
{
    if (!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();  // just in case if PlayerStatBox is not empty

    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        auto Controller = It->Get();
        if (!Controller) continue;

        APlayerStateBase* PlayerState = Cast<APlayerStateBase>(Controller->PlayerState);

        UStatisticWidget* StatisticWidget =
            CreateWidget<UStatisticWidget>(GetWorld(), PlayerStatisticWidgetClass);
        if (!StatisticWidget) continue;

        StatisticWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));

        StatisticWidget->SetKills(FText::FromString(FString::FromInt(PlayerState->GetKillsNum())));
        StatisticWidget->SetDeaths(
            FText::FromString(FString::FromInt(PlayerState->GetDeathsNum())));
        StatisticWidget->SetTeam(FText::FromString(FString::FromInt(PlayerState->GetTeamID())));

        StatisticWidget->SetPlayerIndicatorVisibility(
            Controller->IsPlayerController());  // highlight for player row

        PlayerStatBox->AddChild(StatisticWidget);
    }
}

void UGameOverWidget::OnResetLevel()
{
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}

void UGameOverWidget::OnClickMainMenu()
{
    FName MainMenuName = "MainMenu";
    UGameplayStatics::OpenLevel(this, MainMenuName);
}
