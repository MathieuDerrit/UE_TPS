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
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &UTargetClass::OnOverlapBegin);
	HitBoxLocation = GetOwner()->GetActorLocation();
	HitBox->SetWorldLocation(HitBoxLocation);
	HitBox->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	CurrentRotation = GetOwner()->GetActorRotation();
	HitRotation = CurrentRotation + FRotator(0.0f, -40.0f, 0.0f);
	HitRotationReversed = CurrentRotation + FRotator(0.0f, 40.0f, 0.0f);

	// ...
	
}


// Called every frame
void UTargetClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetClass::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Overlap Test"));
	
	if (isReverse)
	{
		//GetOwner()->SetActorRotation(FRotator(0.0f, 115.0f, 0.0f), ETeleportType::TeleportPhysics);
		GetOwner()->SetActorRotation(HitRotation, ETeleportType::TeleportPhysics);
	}
	else
	{
		//GetOwner()->SetActorRotation(FRotator(0.0f, -98.0f, 0.0f), ETeleportType::TeleportPhysics);
		GetOwner()->SetActorRotation(HitRotationReversed, ETeleportType::TeleportPhysics);
	}

	//FString Test = GetOwner()->GetClass()->GetName();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s"), *Test));
}

