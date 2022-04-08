// Shoot Them Up Game. All Rights Reserved.

#include "AI/EQS/EnvQueryTest_OwningFlag.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Player/STUPlayerState.h"
#include "Flag/STUFlagActor.h"
#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "STUCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogTest, All, All);

UEnvQueryTest_OwningFlag::UEnvQueryTest_OwningFlag(const FObjectInitializer& ObjectInitializer)  //
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::High;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_OwningFlag::RunTest(FEnvQueryInstance& QueryInstance) const
{
    const auto DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const bool bWantsBeTakable = BoolValue.GetValue();

    const auto Player = Cast<ASTUAICharacter>(DataOwner);
    if (!Player) return;

    const auto Controller = Player->GetController<ASTUAIController>();
    if (!Controller) return;

    const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
    if (!PlayerState) return;

    const auto PlayerTeamID = PlayerState->GetTeamID();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());

        const auto Flag = Cast<ASTUFlagActor>(ItemActor);
        if (!Flag) return;

        const auto OwningTeamID = Flag->GetOwningTeamID();
        const auto FlagState = Flag->GetCaptureState();
        const auto TryingToUncapture = Flag->GetTryingToUncapture();
        const auto GoToFlag = PlayerTeamID != OwningTeamID || FlagState != ESTUFlagState::Captured || TryingToUncapture;
        It.SetScore(TestPurpose, FilterType, GoToFlag, bWantsBeTakable);
    }
}
