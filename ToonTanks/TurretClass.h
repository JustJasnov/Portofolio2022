// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TurretClass.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATurretClass : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

protected:
	virtual void BeginPlay() override;
	void CheckFireCondition();

private:

	bool InFireRange();

	class ATank* Tank;

	UPROPERTY(EditAnywhere)
	float fireRange = 700;

	UPROPERTY(EditAnywhere)
	float fireRate = 5;	

	FTimerHandle FireRateTimerHandle;
};
