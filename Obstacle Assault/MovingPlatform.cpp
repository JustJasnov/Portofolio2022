// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();
	FString MyName = GetName();

	if(MovingDirection.Length() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Direction is not normalized"), *MyName);
		MovingDirection = MovingDirection.GetSafeNormal();
	}
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update location via adding 50 units per second to the existing location
	MovePlatform(DeltaTime);
	RotatePlatfrom(DeltaTime);


}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	float DistanceToTheStart = GetDistanceToStart();
	if(ShouldPlatformReturn())
	{
		//Setting new location with Overshoot correction
		float OverShootDistance = DistanceToTheStart - MovingDistance;
		StartingLocation = GetActorLocation() - MovingDirection*OverShootDistance;
		MovingDirection = -MovingDirection;

		FString MyName = GetName();
		UE_LOG(LogTemp, Display, TEXT("%s: Overshoot is: %f"), *MyName, OverShootDistance);
	}

	FVector LocalVector = FVector(GetActorLocation() + (MovingDirection*MovingSpeed*DeltaTime));
	SetActorLocation(LocalVector);
}

void AMovingPlatform::RotatePlatfrom(float DeltaTime)
{
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
	return GetDistanceToStart() > MovingDistance;
}

float AMovingPlatform::GetDistanceToStart() const
{
	return FVector::Dist(GetActorLocation(), StartingLocation);
}