// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUFlagWinningWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUFlagWinningWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRecaptureTimeCountDown() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRestartTimeCountDown() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetWinningTeamID() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsWinningState() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsWonState() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FLinearColor GetWinningTeamColor() const;

    private:
    ASTUFlagGameMode* GetSTUFlagGameMode() const;
};
