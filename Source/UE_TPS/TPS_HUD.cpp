// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_HUD.h"

#include "Blueprint/UserWidget.h"

void ATPS_HUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* MyWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToAdd);
	if (MyWidget)
	{
		MyWidget->AddToViewport(0);
	}
}
