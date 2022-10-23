// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h "
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = baseMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjectileMovement->MaxSpeed = 1300;
	ProjectileMovement->InitialSpeed = 1300;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			LaunchSound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	baseMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* myOwner = GetOwner();
	if(!myOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile does not have owner "));
		Destroy();
		return;
	}
	AController* MyOwnerInstigator = myOwner->GetInstigatorController();

	if (OtherActor && OtherActor != this && OtherActor != myOwner)
	{
		// Particles
		if(HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				this, 
				HitParticles, 
				GetActorLocation(), 
				GetActorRotation()
			);	
		}

		// Sound
		if(HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				HitSound,
				GetActorLocation(),
				GetActorRotation()
			);		
		}

		// Shake
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}	

		UGameplayStatics::ApplyDamage(OtherActor, damageVariable, MyOwnerInstigator, this, UDamageType::StaticClass());
		
	}
	Destroy();
	

}