// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerArea.h"
#include "Component_Mover.h"

// Sets default values for this component's properties
UTriggerArea::UTriggerArea()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UTriggerArea::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UTriggerArea::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* actorWeFound = GetAcceptableActor();
    if (Mover)
    {

        if(actorWeFound)
        {
            UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(actorWeFound->GetRootComponent());

            if(Component)
            {
                Component->SetSimulatePhysics(false);

            }

            actorWeFound->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
            Mover->SetShouldMoveAndRotate(true, true, true, true);

        }
        else
        {
            Mover->SetShouldMoveAndRotate(true, true, false, false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Trigger area: Mover pointer is invalid"));
    }


}

void UTriggerArea::SetMover(UComponent_Mover* newMover)
{
    Mover = newMover;
}

AActor* UTriggerArea::GetAcceptableActor() const
{

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);
    for ( AActor* Actor : OverlappingActors)
    {
        if(Actor->ActorHasTag(myTag) && !Actor->ActorHasTag("Grabbed"))
        {
            return Actor;
        }
        
    }
    return nullptr;
}

