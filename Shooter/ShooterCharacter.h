// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SHOOTERGAME_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector WeaponFocusPoint = FVector(0,0,0);

	UPROPERTY(BlueprintReadOnly)
	AGun* Gun;

	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const;
	
	void Shoot();
	void StopShooting();

	UFUNCTION(BlueprintCallable)
	float GetRemainingHealth() const;

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	int GetCurrentAvailableAmmo() const;
private:

	//Control functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void ReloadPressed();
	void Reload();
	//=================//

	//Gun
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunSubclass;

	//Ammo
	UPROPERTY(EditDefaultsOnly)
	int maxAvailableAmmo;
	int currentAvailableAmmo;



	//Health
	UPROPERTY(EditDefaultsOnly)
	float maxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float currentHealth = 0;
	//=================//

	bool isDead = false;

	//Reload Time variables
	FTimerHandle ReloadTimerHandle;
	float reloadTime = 2;
	bool isReloading = false;
	//============//





};
