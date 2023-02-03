// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BlockingBarrier.generated.h"

UCLASS()
class UE_TPS_API ABlockingBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockingBarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ATPSGameMode* GameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
