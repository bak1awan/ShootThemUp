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

protected:
    TArray<int32> TeamFlagScore{0, 0};

private:
    void RestartFlags();
    void SubscribeToAllFlags();
    void OnFlagCaptured(const int32 TeamID);
    void OnFlagUncaptured(const int32 TeamID);
};
