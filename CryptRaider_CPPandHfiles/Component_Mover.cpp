// Fill out your copyright notice in the Description page of Project Settings.


#include "Component_Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UComponent_Mover::UComponent_Mover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UComponent_Mover::BeginPlay()
{
	Super::BeginPlay();

	origLocation = GetOwner()->GetActorLocation();
	SpeedValue = FVector::Distance(origLocation, origLocation+changeInLocation) / MoveTime;

	origRotation = GetOwner()->GetActorRotation();
	// ...
	
}


// Called every frame
void UComponent_Mover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(ShouldMove)
	{
		MoveTheActor(DeltaTime);
	}

	if(ShouldRotate)
	{
		RotateTheActor(DeltaTime);
	}
}

void UComponent_Mover::MoveTheActor(float DeltaTime)
{
	FVector currentLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation;

	// If move forward is true, then we move to the origLocation+changeInLocation, else we move to the starting location 
	if(MoveForward)
	{
		TargetLocation = origLocation+changeInLocation;
	}
	else
	{
		TargetLocation = origLocation;
	}

	GetOwner()->SetActorLocation(FMath::VInterpConstantTo(currentLocation, TargetLocation, DeltaTime, SpeedValue));

	// If Current location is equal to target location, we stop calling this function to safe some processing power
	if(GetOwner()->GetActorLocation().FVector::Equals(TargetLocation, 0.1)) // Current location is close enough to the target one
	{
		ShouldMove = false;
		MoveForward = !MoveForward;
	}
}

void UComponent_Mover::RotateTheActor(float DeltaTime)
{
	FRotator currentRotation = GetOwner()->GetActorRotation();
	FRotator TargetRotation;

	// If move forward is true, then we move to the origLocation+changeInLocation, else we move to the starting location 
	if(RotateForward)
	{
		TargetRotation = origRotation + changeInRotation;
	}
	else
	{
		TargetRotation = origRotation;
	}
	GetOwner()->SetActorRotation(FMath::RInterpConstantTo(currentRotation, TargetRotation, DeltaTime, 30));

	// If Current location is equal to target location, we stop calling this function to safe some processing power
	if(GetOwner()->GetActorRotation().FRotator::Equals(TargetRotation, 0.1)) // Current rotation is close enough to the target one
	{
		ShouldRotate = false;
		RotateForward = !RotateForward;
	}
}

void UComponent_Mover::SetShouldMoveAndRotate(bool newShouldMove, bool newShouldRotate, bool newMoveForward, bool newRotateForward)
{
    ShouldMove = newShouldMove;
	ShouldRotate = newShouldRotate;
	MoveForward = newMoveForward;
	RotateForward = newRotateForward;
}