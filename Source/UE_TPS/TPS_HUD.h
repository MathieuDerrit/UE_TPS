// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPS_HUD.generated.h"

/**
 * 
 */
UCLASS()
class UE_TPS_API ATPS_HUD : public AHUD
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetToAdd;
	
};
