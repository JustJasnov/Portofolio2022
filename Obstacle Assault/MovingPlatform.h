// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class UNREALLEARNINGKIT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	FVector StartingLocation;

	UPROPERTY(EditAnywhere, Category = "Velocity")
	FVector MovingDirection;

	UPROPERTY(EditAnywhere, Category = "Velocity")
	float MovingSpeed = 120;

	UPROPERTY(EditAnywhere, Category = "Velocity")
	float MovingDistance = 1200;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationVelocity;
	
	void MovePlatform(float DeltaTime);
	void RotatePlatfrom(float DeltaTime);

	bool ShouldPlatformReturn() const;
	float GetDistanceToStart() const;
};
