// Shoot Them Up Game. All Rights Reserved.

#include "STUFlagGameMode.h"
#include "UI/STUFlagWinningWidget.h"

int32 USTUFlagWinningWidget::GetRecaptureTimeCountDown() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return 0;

    return GameMode->GetRecaptureTimeCountDown();
}

int32 USTUFlagWinningWidget::GetRestartTimeCountDown() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return 0;

    return GameMode->GetRestartTimeCountDown();
}

int32 USTUFlagWinningWidget::GetWinningTeamID() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return -1;

    return GameMode->GetWinningTeamID();
}

bool USTUFlagWinningWidget::IsWinningState() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return false;

    return GameMode->IsWinningState();
}

bool USTUFlagWinningWidget::IsWonState() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return false;

    return GameMode->IsWonState();
}

FLinearColor USTUFlagWinningWidget::GetWinningTeamColor() const
{
    const auto GameMode = GetSTUFlagGameMode();
    if (!GameMode) return FLinearColor::White;

    const auto TeamID = GetWinningTeamID();
    if (TeamID == -1) return FLinearColor::White;

    return GameMode->DetermineColorByTeamID(GetWinningTeamID());
}

ASTUFlagGameMode* USTUFlagWinningWidget::GetSTUFlagGameMode() const
{
    return GetWorld() ? Cast<ASTUFlagGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}