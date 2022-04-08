// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_OwningFlag.generated.h"

UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_OwningFlag : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_OwningFlag(const FObjectInitializer& ObjectInitializer);
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FlagActorKeyName = "FlagActor";
};
