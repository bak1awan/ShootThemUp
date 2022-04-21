// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUGameModeBase.h"
#include "STUFlagGameMode.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUFlagGameMode : public ASTUGameModeBase
{
    GENERATED_BODY()
public:
    virtual void StartPlay() override;

    int32 GetRecaptureTimeCountDown() const { return RecaptureTimeCountDown; }
    
    int32 GetRestartTimeCountDown() const { return RestartTimeCountDown; }

    int32 GetWinningTeamID() const { return WinningTeamID; }

    bool IsWinningState() const;

    bool IsWonState() const;

protected:
    TArray<int32> TeamFlagScore{0, 0};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    int32 RecaptureTimeCountDown = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    int32 RestartTimeCountDown = 0;

    virtual void RestartRound() override;
    virtual void GameTimerUpdate() override;

private:
    int32 WinningTeamID = -1;
    FTimerHandle RecaptureTimer;
    FTimerHandle RestartTimer;

    void ResetFlags();
    void SubscribeToAllFlags();
    void OnFlagCaptured(const int32 TeamID);
    void OnFlagUncaptured(const int32 TeamID);
    void RecaptureTimerUpdate();
    void RestartTimerUpdate();
};
