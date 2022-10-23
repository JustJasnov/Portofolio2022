// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTERGAME_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BLueprintReadOnly, Category = "Components")
	class USceneComponent* projectileSpawnPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* muzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* HitSound;

	//Time Handle for shooting speed
	FTimerHandle FireRateTimerHandle;
	float fireRate = 10.8; // Per Second.

	void ManageTheShot();

	bool GunTrace(APawn* OwnerPawn, FHitResult& HitRes);

	UPROPERTY(EditDefaultsOnly)
	int maxAmmo = 30;

	UPROPERTY()
	int currentAmmo;

	UPROPERTY(EditDefaultsOnly)
	float damage = 30;

	UPROPERTY(EditDefaultsOnly)
	float maxRange = 2000;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTheTrigger();
	void StopShooting();
	int Reload(int playerAmmo);

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo() const;
	int GetMaxAmmo() const;
	void SetCurrentAmmo(int newValue);


};
