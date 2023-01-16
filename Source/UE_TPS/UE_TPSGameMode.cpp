// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_TPSGameMode.h"
#include "UE_TPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_TPSGameMode::AUE_TPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
