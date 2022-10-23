// Fill out your copyright notice in the Description page of Project Settings.



#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank(){
    //Super::ABasePawn();

	//Create Spring arm and connect it to the base
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);

	//Create camera
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("RotateRight"), this, &ATank::Rotate);
	PlayerInputComponent->BindAxis(TEXT("RotateTurret"), this, &ATank::RotateTurret);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);


}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controlled Tank: Cast to APlayerController in the BeginPlay failed"));
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(PlayerController)
	{

		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult
		);

		RotateTheTurret(HitResult.ImpactPoint);		
	}

	
}

void ATank::Move(float AxisValue)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = AxisValue*SpeedValue*UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Rotate(float AxisValue)
{
	
	FRotator DeltaRotator = FRotator::ZeroRotator;
	DeltaRotator.Yaw = AxisValue*RotationValue*UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotator);

}

void ATank::RotateTurret(float AxisValue)
{
	
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

APlayerController* ATank::GetTankPlayerController() const
{
	return PlayerController;
}