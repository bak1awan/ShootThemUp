// Shoot Them Up Game. All Rights Reserved.

#include "STUFlagGameMode.h"
#include "STUCoreTypes.h"
#include "Flag/STUFlagActor.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(FlagGameModeLog, All, All);

void ASTUFlagGameMode::StartPlay()
{
    Super::StartPlay();

    SubscribeToAllFlags();
}

bool ASTUFlagGameMode::IsWinningState() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(RecaptureTimer);
}

bool ASTUFlagGameMode::IsWonState() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(RestartTimer);
}

void ASTUFlagGameMode::RestartRound()
{
    TeamFlagScore = {0, 0};
    WinningTeamID = -1;
    ResetFlags();
    Super::RestartRound();
}

void ASTUFlagGameMode::GameTimerUpdate() 
{
    if (--RoundCountDown == 0)
    {
        RestartTimeCountDown = GameData.RestartTime;
        GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &ASTUFlagGameMode::RestartTimerUpdate, 1.0f, true);
    }
}

void ASTUFlagGameMode::ResetFlags()
{
    for (auto Flag : TActorRange<ASTUFlagActor>(GetWorld()))
    {
        Flag->ResetFlag();
    }
}

void ASTUFlagGameMode::SubscribeToAllFlags()
{
    if (!GetWorld()) return;

    for (const auto Flag : TActorRange<ASTUFlagActor>(GetWorld()))
    {
        Flag->OnFlagCaptured.AddUObject(this, &ASTUFlagGameMode::OnFlagCaptured);
        Flag->OnFlagUncaptured.AddUObject(this, &ASTUFlagGameMode::OnFlagUncaptured);
    }
}

void ASTUFlagGameMode::OnFlagCaptured(const int32 TeamID) 
{
    ++TeamFlagScore[TeamID - 1];
    if (TeamFlagScore[TeamID - 1] == 3)
    {
        WinningTeamID = TeamID;
        RecaptureTimeCountDown = GameData.RecaptureTime;
        GetWorld()->GetTimerManager().SetTimer(
            RecaptureTimer, this, &ASTUFlagGameMode::RecaptureTimerUpdate, 1.0f, true);
    }
}

void ASTUFlagGameMode::OnFlagUncaptured(const int32 TeamID) 
{
    --TeamFlagScore[TeamID - 1];
    if (WinningTeamID == TeamID && TeamFlagScore[TeamID - 1] < 3)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecaptureTimer);
        WinningTeamID = -1;
    }
}

void ASTUFlagGameMode::RecaptureTimerUpdate() 
{
    if (--RecaptureTimeCountDown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecaptureTimer);
        RestartTimeCountDown = GameData.RestartTime;
        GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &ASTUFlagGameMode::RestartTimerUpdate, 1.0f, true);
    }
}

void ASTUFlagGameMode::RestartTimerUpdate() 
{
    if (--RestartTimeCountDown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RestartTimer);
        RestartRound();
    }
}
