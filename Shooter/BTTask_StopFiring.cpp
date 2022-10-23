// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopFiring.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_StopFiring::UBTTask_StopFiring()
{
    NodeName = TEXT("Stop Firing");
}

EBTNodeResult::Type UBTTask_StopFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }
    
    AShooterCharacter* shooterPawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(shooterPawn)
    {
        shooterPawn->StopShooting();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}
