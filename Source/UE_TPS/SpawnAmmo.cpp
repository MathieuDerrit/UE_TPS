// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAmmo.h"

// Sets default values
ASpawnAmmo::ASpawnAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup members
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
}

// Called when the game starts or when spawned
void ASpawnAmmo::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < nbToSpawn; i++)
	{
		SpawnActor();
	}
}

bool ASpawnAmmo::SpawnActor()
{
	bool SpawnedActor = false;
	if (ActorsToSpawn.Num() > 0)
	{
		// Calculate the extends of the box
		FBoxSphereBounds BoxBounds = SpawnBox->CalcBounds(GetActorTransform());

		// Compute a random position within the box bounds
		FVector SpawnLocation = BoxBounds.Origin;
		SpawnLocation.X += -BoxBounds.BoxExtent.X + 2 * BoxBounds.BoxExtent.X * FMath::FRand();
		SpawnLocation.Y += -BoxBounds.BoxExtent.Y + 2 * BoxBounds.BoxExtent.Y * FMath::FRand();
		SpawnLocation.Z += -BoxBounds.BoxExtent.Z + 2 * BoxBounds.BoxExtent.Z * FMath::FRand();

		// Spawn the actor
		SpawnedActor = GetWorld()->SpawnActor(ActorsToSpawn[FMath::RandRange(0, ActorsToSpawn.Num() - 1)], &SpawnLocation) != nullptr;
		
	}

	return SpawnedActor;
}
