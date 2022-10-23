// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UComponent_Mover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_Mover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void MoveTheActor(float DeltaTime);

	void RotateTheActor(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SetShouldMoveAndRotate(bool newShouldMove, bool newShouldRotate, bool newMoveForward, bool newRotateForward);

private:
	UPROPERTY(EditAnywhere)
	FVector origLocation;

	UPROPERTY(EditAnywhere)
	FRotator origRotation;

	UPROPERTY(EditAnywhere)
	FRotator changeInRotation;
	
	UPROPERTY(EditAnywhere)
	FVector changeInLocation;

	UPROPERTY(EditAnywhere)
	float MoveTime = 4;

	UPROPERTY(EditAnywhere)
	bool MoveForward = true;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere)
	bool RotateForward = true;	

	UPROPERTY(EditAnywhere)
	bool ShouldRotate = false;

	UPROPERTY(EditAnywhere)
	float SpeedValue;

	UPROPERTY(EditAnywhere)
	float RotationSpeedValue;	
};
