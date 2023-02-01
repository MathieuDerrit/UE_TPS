//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "DestroyBarrier.h"
//
//// Sets default values for this component's properties
//ADestroyBarrier::ADestroyBarrier()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryActorTick.bCanEverTick = true;
//
//	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	RootComponent = Root;
//
//	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
//	Mesh->SetupAttachment(Root);
//	// ...
//}
//
//
//// Called when the game starts
//void ADestroyBarrier::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// ...
//
//}
//
//void ADestroyBarrier::DestroyObject()
//{
//	ATPSGameMode* gm = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	if (gm->Score == 8)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("3.0")));
//		//DestroyComponent();
//	}
//}