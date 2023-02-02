// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "Math/TransformNonVectorized.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE_TPS_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPSGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
		void AddScore(int Points);

	UFUNCTION(BlueprintCallable)
		int GetScore();

	//UFUNCTION()
	//	void DestroyObject();

	UPROPERTY(EditAnywhere)
		int Score;

	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* Barrier0;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<AActor>ActorToSpawn;
};
