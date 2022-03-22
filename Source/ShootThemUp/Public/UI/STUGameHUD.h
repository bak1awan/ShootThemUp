// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidjetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidjetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<ESTUMatchState, UUserWidget*> GameWidjets;

    UPROPERTY()
    UUserWidget* CurrentWidjet = nullptr;

    void DrawCrosshair();
    void OnMatchStateChanged(ESTUMatchState State);
};