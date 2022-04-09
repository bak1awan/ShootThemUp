// Shoot Them Up Game. All Rights Reserved.

#include "Flag/STUFlagActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "Player/STUPlayerState.h"
#include "STUGameModeBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "STUBaseCharacter.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlag, All, All);

ASTUFlagActor::ASTUFlagActor()
{
    PrimaryActorTick.bCanEverTick = false;

    FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlagMesh");
    SetRootComponent(FlagMesh);

    FlagCollision = CreateDefaultSubobject<USphereComponent>("FlagCollision");
    FlagCollision->SetupAttachment(FlagMesh);
    FlagCollision->InitSphereRadius(CollisionRadius);
    FlagCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FlagCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUFlagActor::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(CaptureTimer, this, &ASTUFlagActor::UpdateCaptureTimer, TimerRate, true);
    check(FlagMesh);
    check(FlagCollision);
    SetFlagColor();
}

void ASTUFlagActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    const auto Character = Cast<ASTUBaseCharacter>(OtherActor);
    if (!Character) return;

    AddPlayerToSeize(Character);
}

void ASTUFlagActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    const auto Character = Cast<ASTUBaseCharacter>(OtherActor);
    if (!Character) return;

    RemovePlayerFromSeize(Character);
}

void ASTUFlagActor::SetFlagColor()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
    if (!GameMode) return;
    const auto NewFlagColor =
        CapturedByTeamNumber == -1 ? DefaultFlagColor : TeamFlagColor = GameMode->DetermineColorByTeamID(CapturedByTeamNumber);
    TeamFlagColor = NewFlagColor;
}

void ASTUFlagActor::UpdateFlagColor()
{
    const auto MaterialInst = FlagMesh->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst) return;
    const auto Color = UKismetMathLibrary::LinearColorLerp(DefaultFlagColor, TeamFlagColor, GetFlagCapacityPercent());
    CurrentFlagColor = Color;
    MaterialInst->SetVectorParameterValue("Color", Color);
}

void ASTUFlagActor::AddPlayerToSeize(ACharacter* Player)
{
    const auto PlayerState = GetPlayerState(Player);
    if (!PlayerState) return;
    const auto TeamID = PlayerState->GetTeamID();
    ++TeamPlayerCounter[TeamID - 1];
}

void ASTUFlagActor::ResetFlag()
{
    CurrentFlagColor = DefaultFlagColor;
    CurrentFlagCapacity = 0;
    TeamPlayerCounter = {0, 0};
    CapturedByTeamNumber = -1;
    FlagState = ESTUFlagState::NotCaptured;
}

void ASTUFlagActor::RemovePlayerFromSeize(ACharacter* Player)
{
    const auto PlayerState = GetPlayerState(Player);
    if (!PlayerState)
    {
        UE_LOG(LogFlag, Error, TEXT("Cant get PlayerState"));
        return;
    }
    const auto TeamID = PlayerState->GetTeamID();
    --TeamPlayerCounter[TeamID - 1];
}

ASTUPlayerState* ASTUFlagActor::GetPlayerState(ACharacter* Player)
{
    const auto Controller = Cast<AController>(Player->Controller);
    if (!Controller)
    {
        UE_LOG(LogFlag, Error, TEXT("Cant get controller"));
        return nullptr;
    }
    return Controller->GetPlayerState<ASTUPlayerState>();
}

void ASTUFlagActor::UpdateCaptureTimer()
{
    const int32 TeamDifference = TeamPlayerCounter[0] - TeamPlayerCounter[1];
    int32 TeamDifferenceCaptured;
    if (FlagState == ESTUFlagState::Captured)
    {
        TeamDifferenceCaptured = TeamPlayerCounter[CapturedByTeamNumber - 1] - TeamPlayerCounter[CapturedByTeamNumber % 2];
    }

    switch (FlagState)
    {
        case ESTUFlagState::NotCaptured:
            // if there is more of first team players near the flag
            if (TeamPlayerCounter[0] > TeamPlayerCounter[1])
            {
                // if it was still not started to capture by anyone
                if (CapturedByTeamNumber == -1)
                {
                    CapturedByTeamNumber = 1;  //  make it first team's flag
                    SetFlagColor();
                    CurrentFlagCapacity = FMath::Clamp(
                        CurrentFlagCapacity + TeamDifference * CaptureSpeed, 0, MaxFlagCapacity);  // start to increase capacity
                }
                // if it is already was started to captured by another team - decrease capacity
                else if (CapturedByTeamNumber != 1)
                {
                    CurrentFlagCapacity =
                        FMath::Clamp(CurrentFlagCapacity - (TeamDifference * CaptureSpeed + CaptureSpeed), 0, MaxFlagCapacity);
                }
                // else just increase the capacity
                else
                {
                    CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity + TeamDifference * CaptureSpeed, 0, MaxFlagCapacity);
                }
            }
            // if another team is bigger, the same logic is here for them
            else if (TeamPlayerCounter[0] < TeamPlayerCounter[1])
            {
                if (CapturedByTeamNumber == -1)
                {
                    CapturedByTeamNumber = 2;
                    SetFlagColor();
                    CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity + (-TeamDifference * CaptureSpeed), 0, MaxFlagCapacity);
                }
                else if (CapturedByTeamNumber != 2)
                {
                    CurrentFlagCapacity =
                        FMath::Clamp(CurrentFlagCapacity - (-TeamDifference * CaptureSpeed + CaptureSpeed), 0, MaxFlagCapacity);
                }
                else
                {
                    CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity + (-TeamDifference * CaptureSpeed), 0, MaxFlagCapacity);
                }
            }

            // if there is equal amount of characters we should not increase the capacity
            else if (TeamPlayerCounter[0] == TeamPlayerCounter[1] && TeamPlayerCounter[0] > 0)
            {
                break;
            }

            // if there is nobody near and capacity is more then 0 and it is still not captured -
            // we need to decrease the capacity
            else if (CurrentFlagCapacity > 0)
            {
                CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity - CaptureSpeed, 0, MaxFlagCapacity);
            }

            // if Current flag capacity get to 0, we need to change the captured team number of the flag to -1 (restore to default)
            if (CurrentFlagCapacity == 0)
            {
                CapturedByTeamNumber = -1;
                SetFlagColor();
            }

            // If one team captured the flag - change state
            if (CurrentFlagCapacity == MaxFlagCapacity)
            {
                FlagState = ESTUFlagState::Captured;
                OnFlagCaptured.Broadcast(CapturedByTeamNumber);
            }
            break;

        case ESTUFlagState::Captured:
            // if amount of enemy players is bigger then amount of captured players - start to capture
            if (TeamPlayerCounter[CapturedByTeamNumber - 1] < TeamPlayerCounter[CapturedByTeamNumber % 2])
            {
                EnemyTryingToUncapture = true;
                CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity + (TeamDifferenceCaptured * CaptureSpeed), 0, MaxFlagCapacity);
            }
            // if amount of captured players is bigger then amount of enemy players and second one have already captured some capacity
            // start to restore capacity
            else if (TeamPlayerCounter[CapturedByTeamNumber - 1] > TeamPlayerCounter[CapturedByTeamNumber % 2] &&
                     CurrentFlagCapacity < MaxFlagCapacity)
            {
                EnemyTryingToUncapture = false;
                CurrentFlagCapacity =
                    FMath::Clamp(CurrentFlagCapacity + (TeamDifferenceCaptured * CaptureSpeed + CaptureSpeed), 0, MaxFlagCapacity);
            }
            // if there is no players near the flag but before enemy players have captured a little bit of capacity - restore it
            else if (TeamPlayerCounter[CapturedByTeamNumber - 1] == TeamPlayerCounter[CapturedByTeamNumber % 2] &&
                     TeamPlayerCounter[CapturedByTeamNumber - 1] == 0)
            {
                EnemyTryingToUncapture = false;
                CurrentFlagCapacity = FMath::Clamp(CurrentFlagCapacity + CaptureSpeed, 0, MaxFlagCapacity);
            }
            // if there is equal amount of players and capacity is less then max value - do nothing
            else if (TeamPlayerCounter[CapturedByTeamNumber - 1] == TeamPlayerCounter[CapturedByTeamNumber % 2])
            {
                EnemyTryingToUncapture = true;
                break;
            }

            // if enemy team captured all the capacity of owner team - change state
            if (CurrentFlagCapacity == 0)
            {
                EnemyTryingToUncapture = false;
                OnFlagUncaptured.Broadcast(CapturedByTeamNumber);
                CapturedByTeamNumber = -1;
                SetFlagColor();
                FlagState = ESTUFlagState::NotCaptured;
            }
            break;
    }
    UpdateFlagColor();
}
