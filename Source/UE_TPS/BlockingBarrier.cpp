// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"
#include "BlockingBarrier.h"


// Sets default values
ABlockingBarrier::ABlockingBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlockingBarrier::BeginPlay()
{
	Super::BeginPlay();
	//
	//GetWorld()->SpawnActor<AActor>(ActorSpawning, GetActorTransform());
}

// Called every frame
void ABlockingBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ABlockingBarrier::Destroying()
//{
//	ATPSGameMode* gm = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	if ( gm->Score == 8)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TEST_DESTROY")));
//		//Destroy();
//	}
//}