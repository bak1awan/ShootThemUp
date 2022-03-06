// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STURiffleWeapon.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"

void ASTURiffleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }
    

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
    DecreaseAmmo();
}

void ASTURiffleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURiffleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURiffleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

bool ASTURiffleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    TraceEnd = TraceStart + FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(BulletSpred)) * TraceMaxDistance;
    return true;
}

void ASTURiffleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;
    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
