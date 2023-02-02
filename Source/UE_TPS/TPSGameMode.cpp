// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"
#include "UObject/ConstructorHelpers.h"

ATPSGameMode::ATPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	

	//Barrier0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierBlock"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh>Barrier(TEXT("Game/SoulCity/Environment/Meshes/NewNavi/Handrails/SM_slum_handrails_handrails_12"));
	//((UStaticMeshComponent*)Barrier0)->SetStaticMesh(Barrier.Object);
	//Barrier0->SetWorldLocationAndRotation(FVector(100.0f, 0.0f, 0.0f), FQuat(0.0), false, 0, ETeleportType::None);

	//UWorld* const world = GetWorld();
	//world->Spawb

/*	UStaticMesh* BlockingBarrier = NewObject<UStaticMesh>(this, TEXT("BarrierStaticMesh"));
	SetStaticMesh(BlockingBarrier)*/;

	//GetWorld()->SpawnActor<AActor>(ActorToSpawn, FTransform(FVector(0, 0, 0)));
}

void ATPSGameMode::AddScore(int Points) 
{
	Score += Points;

	if (Score == 8)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TEST")));
		
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%lld"), Score));
}

int ATPSGameMode::GetScore()
{
	return Score;
}
/*
void AUE_TPSCharacter::Countdown()
{
	if (Seconds != 0)
	{
		Seconds = Seconds - 1;
	}
	else
	{
		if (Minutes == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TIMER END")));
			UGameplayStatics::OpenLevel(GetWorld(), "Menu");
		}
		else
		{
			Minutes = Minutes - 1;
			Seconds = 59;
		}
	}
}*/