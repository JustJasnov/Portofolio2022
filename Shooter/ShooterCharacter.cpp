// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Gun = GetWorld()->SpawnActor<AGun>(GunSubclass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	currentAvailableAmmo = maxAvailableAmmo;
	currentHealth = maxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Control bind //
	//AXIS:
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::Turn);
	//ACTIONS:
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AShooterCharacter::StopShooting);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Released, this, &AShooterCharacter::ReloadPressed);
	//=================//

}




//// Movement Control
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}
void AShooterCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}
//=======================//


////Shoot and Gun Control
void AShooterCharacter::Shoot()
{
	if(Gun)
	{
		if(!isReloading && !isDead)
		{
			if(Gun->GetCurrentAmmo() <= 0)
			{
				ReloadPressed();
				return;
			}
			Gun->PullTheTrigger();
		}
		else
		{
			StopShooting();
		}

	}
	
}

void AShooterCharacter::StopShooting()
{
	if(Gun)
	{
		Gun->StopShooting();
	}
}

void AShooterCharacter::ReloadPressed()
{
	//Checks if already reloading. Prevents Reload key spam to help key spammers in nervous situations :)
	if(isReloading || isDead)
	{
		return;
	}

	// If gun exists and current ammo is lower then maximum, only then we reload.
	if(Gun && Gun->GetCurrentAmmo() < Gun->GetMaxAmmo())
	{

		//Sets timer for reload. TODO: Reload Time should be in sync with reload animation
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AShooterCharacter::Reload, reloadTime, false);
		isReloading = true;		

	}

}

void AShooterCharacter::Reload()
{

	isReloading = false;
	if(Gun && !isDead)
	{
		currentAvailableAmmo = Gun->Reload(currentAvailableAmmo);
	}
}

int AShooterCharacter::GetCurrentAvailableAmmo() const
{
	return currentAvailableAmmo;
}
int AShooterCharacter::GetCurrentAmmo() const
{
	if(Gun)
	{
		return Gun->GetCurrentAmmo();
	}
	else
	{
		return 0;
	}
}


float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	currentHealth = FMath::Clamp(currentHealth - damageApplied, 0, maxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Myhealth: %f"), currentHealth);

	if(currentHealth <= 0)
	{
		isDead = true;
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StopShooting();
		AShooterGameGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameGameModeBase>();
		if(GameMode)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}

	return damageApplied;
}

bool AShooterCharacter::GetIsDead() const
{
	return isDead;
}
//=======================//

float AShooterCharacter::GetRemainingHealth() const
{
    return (currentHealth / maxHealth);
}