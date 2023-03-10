// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Dev/CustomTypes.h"
#include "CitadelGameModeBase.generated.h"

class AAIController;
UCLASS()
class CITADEL_API ACitadelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACitadelGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;

	virtual void StartPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(
		AController* InController) override;

	virtual void ConfirmKill(
		AController* KillerController, AController* VictimController);

	int32 GetCurrentRoundNum() { return CurrentRound; };
	int32 GetRoundCountdown() { return RoundCountdown; };
	int32 GetTotalRoundsNum() { return GameData.RoundsNum; };
	void RequestRespawn(AController* Controller);

    virtual bool SetPause(APlayerController* PC,
        FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

    virtual bool ClearPause() override;

    CitadelMatchState GetMatchState() { return MatchState; };

protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

private:
	CitadelMatchState MatchState = CitadelMatchState::WaitingToStart;
	int32 CurrentRound = 1;
	int32 RoundCountdown = 0;
	FTimerHandle RoundTimerHandle;
    bool IsPauseSet = false;

    void StartNewRound();
	void UpdateRoundTimer();
	void SpawnBots();
	void ResetPlayers();
	void ResetOnePlayer(AController* PlayerController);

	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void CreateTeamsInfo();
	void SetPlayerColor(AController* Controller);

	void PrintPlayerStatistic();
	void StartRespawnProcess(AController* Controller);
	void FinishGame();
	void DisableAllPawns();

	void SetMatchState(CitadelMatchState State);

	
};
