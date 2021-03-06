// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    FOnMatchStateChangedSignature OnMatchStateChanged;

    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    void Killed(AController* KillerController, AController* VictimController);

    FGameData GetGameData() const { return GameData; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void RespawnRequest(AController* Controller);
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    int32 RoundCountDown = 0;

    virtual void RestartRound();
    virtual void GameTimerUpdate();

private:
    int32 CurrentRound = 1;
    
    FTimerHandle GameRoundTimerHandle;
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;

    void SetMatchState(ESTUMatchState State);

    void SpawnBots();
    void StartRound();
    

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    
    void SetPlayerColor(AController* Controller);

    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();
    void StopAllFire();
};
