// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURiffleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURiffleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURiffleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void Zoom(bool Enabled) override;

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot() override;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpred = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FOVZoomAngle = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    float DefaultCameraFOV = 90.0f;

    FTimerHandle ShotTimerHandle;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    UPROPERTY()
    UAudioComponent* FireAudioComponent;

    void MakeDamage(const FHitResult& HitResult);
    // void InitMuzzleFX();
    void InitFX();
    // void SetMuzzleFXVisibility(bool Visible);
    void SetFXActive(bool IsActive);

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    AController* GetController() const;
};