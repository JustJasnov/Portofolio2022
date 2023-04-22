// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Color.h"
#include "Components/PointLightComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_Lightbulb.generated.h"

UDELEGATE(NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChanged, FColor, newColor);

UDELEGATE(NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulbDestroyed);

UCLASS()
class CPLUSPLUS_LIGHTBULB_API ABP_Lightbulb : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ABP_Lightbulb();

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FOnColorChanged bulbColorChangedDispatcher;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FOnBulbDestroyed bulbDestroyedDispatcher;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Геттеры
	UFUNCTION(BlueprintCallable)
	bool getIsOn() const;

	UFUNCTION(BlueprintCallable)
	bool getIsAlive() const;

	UFUNCTION(BlueprintCallable)
	struct FColor getMyColor() const;

	// Сеттеры
	UFUNCTION(BlueprintCallable)
	void SetLight(bool shouldBeOn);

	UFUNCTION(BlueprintCallable)
	void ToggleBulb();	

	UFUNCTION(BlueprintCallable)
	void SetMyColor(struct FColor newColor);

	UFUNCTION(BlueprintCallable)
	void RandomizeMyColor();

	// Пепезапись функции нанесения урона.
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Для репликации
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_isAliveUpdated)
	bool isAlive = true;

	UPROPERTY(ReplicatedUsing = OnRep_isOnUpdated)
	bool isOn = true;

	UPROPERTY(ReplicatedUsing = OnRep_ColorUpdated)
	struct FColor myColor = FColor(255, 255, 255,255);

	// Функции обновления внешнего вида лампочки для репликации
	UFUNCTION()
	void OnRep_isAliveUpdated();

	UFUNCTION()
	void OnRep_isOnUpdated();

	UFUNCTION()
	void OnRep_ColorUpdated();	

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* baseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UPointLightComponent* thouLight;	

};
