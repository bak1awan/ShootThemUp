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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* FlagMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* FlagCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    int32 MaxFlagCapacity = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlagProperties")
    int32 CaptureSpeed = 5;


    ESTUFlagState FlagState = ESTUFlagState::NotCaptured;

    int32 CapturedByTeamNumber = -1;

    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
    FTimerHandle CaptureTimer;

    FVector2D TeamPlayerCounter = FVector2D::ZeroVector;
    int32 CurrentFlagCapacity = 0;

    void AddPlayerToSeize(ACharacter* Player);
    void RemovePlayerFromSeize(ACharacter* Player);
    ASTUPlayerState* GetPlayerState(ACharacter* Player);
    void UpdateCaptureTimer();
};
