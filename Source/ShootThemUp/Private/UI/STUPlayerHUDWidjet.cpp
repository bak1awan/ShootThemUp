// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPlayerHUDWidjet.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"

int32 USTUPlayerHUDWidjet::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer();
    if (!Controller) return 0;
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

void USTUPlayerHUDWidjet::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercents() > PercentColorThreshold ? GoodColor : BadColor);
    }
}

float USTUPlayerHUDWidjet::GetHealthPercents() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidjet::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;
    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidjet::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidjet::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidjet::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

FString USTUPlayerHUDWidjet::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;  // max amount of '0' we could add before bullets number
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();

    if (SymbolsNumToAdd > 0) BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);

    return BulletStr;
}

void USTUPlayerHUDWidjet::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidjet::OnHealthChanged);
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidjet::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
        if (!IsAnimationPlaying(DamageAnimation)) PlayAnimation(DamageAnimation);
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidjet::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidjet::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}
