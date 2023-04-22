// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_Lightbulb.h"
#include "Math/Color.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameEngine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Принцип работы: Лампочка на сервере имеет весь функционал, а игрок на клиенте изменить лампочку не может, даже локально.
// Для этого при вызове функций иземнения внешнего вида лампочки идёт проверка на соответствие роли Actor'а роли Authority.
// Для репликаций предусмотрены функции On_rep, которые просто обновляют вид лампочки на клиентах, поскольку при синхронизации переменных автоматическое изменение вида лампочки не происходит.
// Для диспатчера используется NetMulticast.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Sets default values
ABP_Lightbulb::ABP_Lightbulb()
{
	PrimaryActorTick.bCanEverTick = false;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = baseMesh;

	thouLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Component"));
	thouLight->SetupAttachment(RootComponent);


	SetReplicates(true);
}


// Called when the game starts or when spawned
void ABP_Lightbulb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_Lightbulb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ГЕТТЕРЫ
bool ABP_Lightbulb::getIsOn() const
{
	return isOn;

}

bool ABP_Lightbulb::getIsAlive() const
{
	return isAlive;

}

struct FColor ABP_Lightbulb::getMyColor() const
{
	return myColor;

}

// СЕТТЕРЫ И ФУНКЦИОНАЛ
void ABP_Lightbulb::SetLight(bool shouldBeOn)
{
	if (GetLocalRole() != ROLE_Authority){return;}

	if(thouLight)
	{
		isOn = shouldBeOn && isAlive; // Если лампочка разбита, она не запустится
		thouLight->SetVisibility(isOn);
	}
	
}

void ABP_Lightbulb::ToggleBulb()
{
	if (GetLocalRole() != ROLE_Authority){return;}

	if(thouLight)
	{	
		isOn = !isOn && isAlive; // Если лампочка разбита, она не запустится
		thouLight->SetVisibility(isOn);
	}
}

void ABP_Lightbulb::SetMyColor(struct FColor newColor)
{
	if (GetLocalRole() != ROLE_Authority){return;}

	if(thouLight)
	{
		myColor = newColor;
		thouLight->SetLightColor(myColor);

		bulbColorChangedDispatcher.Broadcast(myColor);
	}
}

// ПРОСТЕНЬКИЙ РАНДОМАЙЗЕР ЦВЕТА
void ABP_Lightbulb::RandomizeMyColor()
{
	if (GetLocalRole() != ROLE_Authority){return;}

	SetMyColor(FColor(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255), 255)); //Случайный цвет лампочки
	
}

// ПОЛУЧЕНИЕ УРОНА
float ABP_Lightbulb::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetLocalRole() != ROLE_Authority){return 0;}
	if(DamageAmount<=0){return 0;}

	isAlive = false;
	SetLight(false);

	bulbDestroyedDispatcher.Broadcast();

	return DamageAmount;
}

///////////////////////////////////////////////////////
///////////////////// РЕПЛИКАЦИЯ //////////////////////
///////////////////////////////////////////////////////

//Привязка переменных для репликации.
void ABP_Lightbulb::GetLifetimeReplicatedProps( TArray<FLifetimeProperty> & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( ABP_Lightbulb, isAlive )
	DOREPLIFETIME( ABP_Lightbulb, isOn )
	DOREPLIFETIME( ABP_Lightbulb, myColor )

}

//Обновление внешнего вида лампочки на клиентах при изменении значения переменных на сервере.
void ABP_Lightbulb::OnRep_isAliveUpdated()
{
	if(thouLight)
	{
		thouLight->SetVisibility(isOn && isAlive);
	}
}

void ABP_Lightbulb::OnRep_isOnUpdated()
{
	if(thouLight)
	{
		thouLight->SetVisibility(isOn && isAlive);
	}
}

void ABP_Lightbulb::OnRep_ColorUpdated()
{
	if(thouLight)
	{
		thouLight->SetLightColor(myColor);
		
	}
}
