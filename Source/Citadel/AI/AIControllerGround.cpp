// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerGround.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "AI/Components/PerceptionComponent.h"
#include "AI/AIGround.h"
#include "Components/RespawnComponent.h"


AAIControllerGround::AAIControllerGround()
{
    PerceptionComponentGround = CreateDefaultSubobject<UPerceptionComponent>(
        TEXT("PerceptionComponentGround"));
    SetPerceptionComponent(*PerceptionComponentGround);

    RespawnComponent = CreateDefaultSubobject<URespawnComponent>(
                TEXT("RespawnComponent"));

    bWantsPlayerState = true; // required for AI respawn system
}

void AAIControllerGround::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetFocus(GetEnemyFromBlackboard());
}

void AAIControllerGround::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AIGround = Cast<AAIGround>(InPawn);
    if (AIGround)
    {
        RunBehaviorTree(AIGround->BehaviorTreeAsset);
    }
}

AActor* AAIControllerGround::GetEnemyFromBlackboard()
{
    Blackboard = GetBlackboardComponent();
    if (!Blackboard) return nullptr;

    return Cast<AActor>(Blackboard->GetValueAsObject(EnemyVarNameFromBlackboard));
}
