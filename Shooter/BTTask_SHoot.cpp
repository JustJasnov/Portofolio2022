// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SHoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }
    
    AShooterCharacter* shooterPawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(shooterPawn)
    {
        shooterPawn->Shoot();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}