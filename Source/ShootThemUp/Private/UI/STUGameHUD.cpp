// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "STUGameModeBase.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidjets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidjetClass));
    GameWidjets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidjetClass));

    for (auto GameWidjetPair : GameWidjets)
    {
        const auto GameWidjet = GameWidjetPair.Value;
        if (!GameWidjet) continue;

        GameWidjet->AddToViewport();
        GameWidjet->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode) GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
    }
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State) 
{
    if (CurrentWidjet) CurrentWidjet->SetVisibility(ESlateVisibility::Hidden); // won't be called on the first time

    if (GameWidjets.Contains(State)) CurrentWidjet = GameWidjets[State];

    if (CurrentWidjet) CurrentWidjet->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogSTUGameHUD, Warning, TEXT("Match state changed to %s!"), *UEnum::GetValueAsString(State));
}

void ASTUGameHUD::DrawCrosshair()
{

    TInterval<float> Center(Canvas->SizeX * 0.5, Canvas->SizeY * 0.5);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}