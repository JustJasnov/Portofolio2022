// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
	#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Mesh->SetupAttachment(Root);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	projectileSpawnPoint->SetupAttachment(Mesh);
	//================//
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	currentAmmo = maxAmmo;
	 
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTheTrigger()
{
	if(GetOwner())
	{
		//Trigger is pulled. Timer is set. It will call ManageTheShot function every 1/fireRate seconds.
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::ManageTheShot, 1/fireRate, true);
	}

}

void AGun::ManageTheShot()
{
	if(currentAmmo > 0)
	{
		currentAmmo--;
		UGameplayStatics::SpawnEmitterAttached(muzzleFlash, projectileSpawnPoint);
		UGameplayStatics::SpawnSoundAttached(ShootSound, projectileSpawnPoint);

		// Trace section
		APawn* OwnerPawn = Cast<APawn>(GetOwner()); // Check is ownerPawn is APawn
		if(OwnerPawn && OwnerPawn->GetController())
		{
			// Trace
			FHitResult HitRes;
			bool bSuccess = GunTrace(OwnerPawn, HitRes);

			if(bSuccess)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitRes.Location, HitRes.Normal.Rotation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitRes.Location);		
				//Debug sphere to show where did we shoot.
				//DrawDebugSphere(GetWorld(),	HitRes.ImpactPoint,	10,	16,	FColor::Red, true, 0);
				
				AActor* HitActor = HitRes.GetActor();
				if(HitActor)
				{
					FPointDamageEvent damageEvent(damage, HitRes, HitRes.Normal, nullptr);
					HitActor->TakeDamage(damage, damageEvent, OwnerPawn->GetController(), this);
				}
			}




		}
	
	}
	else
	{
		StopShooting();		
	}
}

bool AGun::GunTrace(APawn* OwnerPawn, FHitResult& HitRes)
{
	FVector Location; // Player view location.
	FRotator Rotation; // Player view Rotation
	OwnerPawn->GetController()->GetPlayerViewPoint(Location, Rotation); // Get player view

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel
	(
		HitRes,
		projectileSpawnPoint->GetComponentLocation(),
		Location + Rotation.Vector()*maxRange,
		ECC_GameTraceChannel1
	);
}

void AGun::StopShooting()
{
	//Clear timer Manager. Gun is not able to shoot
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
}


// Ammo getters. They need to make Reload Function public, but keep variables private.
int AGun::GetCurrentAmmo() const
{
	return currentAmmo;
}

int AGun::GetMaxAmmo() const
{
	return maxAmmo;
}
void AGun::SetCurrentAmmo(int newValue)
{
	currentAmmo = newValue;
}
//================//


int AGun::Reload(int playerAmmo)
{
	// If player has less or equal ammo to fully restore the magazine
	if(playerAmmo <= GetMaxAmmo() - GetCurrentAmmo())
	{
		SetCurrentAmmo(GetCurrentAmmo() + playerAmmo);
		return 0;
	}
	else
	{
		int substractedValue = GetMaxAmmo() - GetCurrentAmmo();
		SetCurrentAmmo(GetMaxAmmo());
		return playerAmmo-substractedValue;
	}
}