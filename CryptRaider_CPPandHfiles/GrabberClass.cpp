// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberClass.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabberClass::UGrabberClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberClass::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetPhysicsHandle();
	// ...
	
}


// Called every frame
void UGrabberClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float time = GetWorld()->TimeSeconds;
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->SetTargetLocationAndRotation(
			GetComponentLocation() + GetForwardVector()*grabDistance/2 + GetUpVector()*15, 
			GetComponentRotation()
		);
	}


}

void UGrabberClass::PickUp()
{

	
	if (!PhysicsHandle)
	{
		return;
	}


	FHitResult HitResult;
	if(GetGrabbableInReach(HitResult))
	{

		HitResult.GetComponent()->WakeAllRigidBodies();
		HitResult.GetComponent()->SetSimulatePhysics(true);

		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			10,
			10,
			FColor::Blue,
			false,
			5
		);	
	
		UE_LOG(LogTemp, Display, TEXT("%s"), *HitResult.GetActor()->GetActorNameOrLabel());

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			HitResult.GetComponent()->GetComponentRotation()
		);

	}
}

void UGrabberClass::Release()
{
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");

		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp, Display, TEXT("Released"));
}

UPhysicsHandleComponent* UGrabberClass::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber did not find PhysicsHandle component"));
	}
	return Result;
}

bool UGrabberClass::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector EndPoint = GetComponentLocation() + GetForwardVector()*grabDistance;
	DrawDebugLine(GetWorld(), GetComponentLocation() , EndPoint, FColor::Red);

	
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		GetComponentLocation(),
		EndPoint,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(grabRadius)
	);
}