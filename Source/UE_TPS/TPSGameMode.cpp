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
}

void ATPSGameMode::AddScore(int Points) 
{
	Score += Points;

	 if (Score == 35)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "VictoryMenu");
	}
}

int ATPSGameMode::GetScore()
{
	return Score;
}