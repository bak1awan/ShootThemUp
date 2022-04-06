// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUFlagWidget.generated.h"

class ASTUFlagActor;

UCLASS()
class SHOOTTHEMUP_API USTUFlagWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetFlag(ASTUFlagActor* NewFlag) { Flag = NewFlag; }

    UFUNCTION(BlueprintPure, Category = "UI")
    float GetFlagCapacityPercent();

    UFUNCTION(BlueprintPure, Category = "UI")
    FLinearColor GetFlagColor() const;

protected:
    ASTUFlagActor* Flag = nullptr;
};
