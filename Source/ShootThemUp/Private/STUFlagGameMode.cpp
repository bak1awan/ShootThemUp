// Shoot Them Up Game. All Rights Reserved.

#include "STUFlagGameMode.h"
#include "STUCoreTypes.h"
#include "Flag/STUFlagActor.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

void ASTUFlagGameMode::StartPlay()
{
    Super::StartPlay();

    SubscribeToAllFlags();
}

void ASTUFlagGameMode::RestartFlags() 
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
        TeamFlagScore = {0, 0};
        RestartFlags();
        RestartRound();
    }
}

void ASTUFlagGameMode::OnFlagUncaptured(const int32 TeamID) 
{
    --TeamFlagScore[TeamID - 1];
}
