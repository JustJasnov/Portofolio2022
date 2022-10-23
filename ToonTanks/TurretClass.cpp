// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretClass.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATurretClass::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(!Tank)
    {
        UE_LOG(LogTemp, Error, TEXT("Turret class BeginPlay: Cast failed"));
    }
    
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretClass::CheckFireCondition, fireRate, true);
}

void ATurretClass::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(InFireRange())
    {
        RotateTheTurret(Tank->GetActorLocation());
    }

}

void ATurretClass::CheckFireCondition()
{
    if(InFireRange())
    {
        Fire();
    }
    
}

bool ATurretClass::InFireRange()
{
    return (Tank && FVector::Dist(Tank->GetActorLocation(), GetActorLocation()) <= fireRange);
}

void ATurretClass::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}