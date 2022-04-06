// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUFlagActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ASTUPlayerState;
class ACharacter;

UCLASS()
class SHOOTTHEMUP_API ASTUFlagActor : public AActor
{
    GENERATED_BODY()

public:
    ASTUFlagActor();

    FOnFlagCapturedSignature OnFlagCaptured;
    FOnFlagUncapturedSignature OnFlagUncaptured;

    float GetFlagCapacityPercent() const {return static_cast<float>(CurrentFlagCapacity) / MaxFlagCapacity; }
    FLinearColor GetFlagColor() const { return CurrentFlagColor; }

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    FLinearColor DefaultFlagColor = FLinearColor::White;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* FlagMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* FlagCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    float CollisionRadius = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    int32 MaxFlagCapacity = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    int32 CaptureSpeed = 5;

    FLinearColor TeamFlagColor;
    FLinearColor CurrentFlagColor;

    ESTUFlagState FlagState = ESTUFlagState::NotCaptured;

    int32 CapturedByTeamNumber = -1;

    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

    void SetFlagColor();

    void UpdateFlagColor();

private:
    FTimerHandle CaptureTimer;

    TArray<int32> TeamPlayerCounter{0, 0};
    int32 CurrentFlagCapacity = 0;

    void AddPlayerToSeize(ACharacter* Player);
    void RemovePlayerFromSeize(ACharacter* Player);
    ASTUPlayerState* GetPlayerState(ACharacter* Player);
    void UpdateCaptureTimer();
};
