// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetClass.h"
#include "Math/Rotator.h"

// Sets default values for this component's properties
UTargetClass::UTargetClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	 
	// ...
}


// Called when the game starts
void UTargetClass::BeginPlay()
{
	Super::BeginPlay();
	CurrentRotation = GetOwner()->GetActorRotation();
	HitRotation = CurrentRotation + FRotator(0.0f, -40.0f, 0.0f);
	HitRotationReversed = CurrentRotation + FRotator(0.0f, 40.0f, 0.0f);
	HitRotationVertical = CurrentRotation + FRotator(0.0f, 0.0f, -50.0f);
	
	// ...
	
}


// Called every frame
void UTargetClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetClass::Hit()
{
	if (!isReverse)
	{
		if (isVertical)
		{
			GetOwner()->SetActorRotation(HitRotationVertical, ETeleportType::TeleportPhysics);
		}
		
		else 
		{
			GetOwner()->SetActorRotation(HitRotationReversed, ETeleportType::TeleportPhysics);
		}
	}
	else
	{
		GetOwner()->SetActorRotation(HitRotation, ETeleportType::TeleportPhysics);
	}

	Scoring();
}

void UTargetClass::Scoring()
{
	ATPSGameMode* gm = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (isHit == false) {
		gm->AddScore(1);
		isHit = true;
	}
}