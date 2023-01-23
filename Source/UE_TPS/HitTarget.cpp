// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTarget.h"
#include "Components/BoxComponent.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White,String)

// Sets default values for this component's properties
UHitTarget::UHitTarget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = false;

	// ...
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
}


// Called when the game starts
void UHitTarget::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentHit.AddDynamic(this, &UHitTarget::OnHit);

	// ...
	
}

void AHitTarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PrintString(FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
}