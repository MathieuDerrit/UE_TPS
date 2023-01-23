// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_TPSGameMode.h"
#include "UE_TPSCharacter.h"
#include "TPS_HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AUE_TPSGameMode::AUE_TPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_Soldier(TEXT("/Game/Characters/Soldier/WBP_Soldier"));
	if (WBP_Soldier.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT FOUND ::::"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FOUNDED ::::"));
	}
	
}
