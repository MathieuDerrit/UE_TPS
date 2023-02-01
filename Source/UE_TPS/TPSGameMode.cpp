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


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	

	Barrier0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierBlock"));
	ConstructorHelpers::FClassFinder<UStaticMeshComponent> Barrier(TEXT("Game/SoulCity/Environment/Meshes/NewNavi/Handrails/SM_slum_handrails_handrails_12"));
	//if (Barrier.Succeeded()) {
	//	((UStaticMeshComponent*)Barrier0)->SetStaticMesh(Barrier.Class);
	//}
	Barrier0->SetWorldLocationAndRotation(FVector(100.0f, 0.0f, 0.0f), FQuat(0.0), false, 0, ETeleportType::None);
}

void ATPSGameMode::AddScore(int Points) 
{
	Score += Points;

	if (Score >= 8)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TEST")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%lld"), Score));
}

int ATPSGameMode::GetScore()
{
	return Score;
}