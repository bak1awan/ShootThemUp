// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUHealthPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    UE_LOG(LogHealthPickup, Warning, TEXT("Health was picked up!"));
    return true;
}
