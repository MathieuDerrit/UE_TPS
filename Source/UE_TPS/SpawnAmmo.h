// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnAmmo.generated.h"

UCLASS()
class UE_TPS_API ASpawnAmmo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Will spawn an actor of the specified class now
	UFUNCTION(BlueprintCallable)
		bool SpawnActor();

public:
	// Actor class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AActor>> ActorsToSpawn;

	// Average time between spawns (without random)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AvgSpawnTime = 5.f;
	// Random +/- offset of the spawn time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RandomSpawnTimeOffset = 1.f;

private:
	// Box in which we will spawn the actors
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* SpawnBox;

	// Indicates that the actor should spawn actors
	UPROPERTY(EditAnywhere)
		bool ShouldSpawn = true;

	// Indicates that the actor should spawn actors
	UPROPERTY(EditAnywhere)
		int nbToSpawn = 25;

	// Helper for timing
	FTimerHandle SpawnTimerHandle;
};