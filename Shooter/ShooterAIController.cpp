// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(aiBehavior)
    {
        RunBehaviorTree(aiBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }

}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    
}

bool AShooterAIController::isDead() const
{
    AShooterCharacter* ControllerCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControllerCharacter)
    {
        return ControllerCharacter->GetIsDead();
    }

    return true;
}

