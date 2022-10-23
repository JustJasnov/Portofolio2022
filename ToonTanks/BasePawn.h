// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void HandleDestruction();

protected:
	void RotateTheTurret(FVector TargetLocation);
	void Fire();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCapsuleComponent* capsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* baseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* turretMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* projectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
};
