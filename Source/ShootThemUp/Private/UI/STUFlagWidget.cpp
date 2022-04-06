// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUFlagWidget.h"
#include "Flag/STUFlagActor.h"

float USTUFlagWidget::GetFlagCapacityPercent()
{
    if (!Flag) return 0.0f;

    return Flag->GetFlagCapacityPercent();
}

FLinearColor USTUFlagWidget::GetFlagColor() const
{
    if (!Flag) return FLinearColor::White;
    return Flag->GetFlagColor();
}
