// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockingBarrier.h"


// Sets default values
ABlockingBarrier::ABlockingBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlockingBarrier::BeginPlay()
{
	Super::BeginPlay();
	ATPSGameMode* gm = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode = gm;

}

// Called every frame
void ABlockingBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameMode->Score == 8)
	{
		Destroy();
	}

}