// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* cameraComponent;

	void Move(float AxisValue);
	void Rotate(float AxisValue);
	void RotateTurret(float AxisValue);	

	UPROPERTY(EditAnywhere, Category = "Speed")
	float SpeedValue = 400;	

	UPROPERTY(EditAnywhere, Category = "Speed")
	float RotationValue = 50;

	APlayerController* PlayerController;

	bool bAlive = true;
};
