// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "UE_TPSCharacter.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	/*
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	*/
	//Create activate trigger box
	activateBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Activate Box"));
	activateBox->InitBoxExtent(FVector(20, 20, 20));
	//activateBox->SetBoxExtent(FVector(10,10,10));
	activateBox->SetGenerateOverlapEvents(true);
	activateBox->SetupAttachment(Mesh);
	activateBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	activateBox->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnOverlapBegin);
	activateBox->bHiddenInGame = false;

}

void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUE_TPSCharacter* Player = Cast <AUE_TPSCharacter>(OtherActor);
	if (Player) {
		CollectAmmo(Player);
	}
	
}

bool AAmmo::CollectAmmo(AUE_TPSCharacter* Character)
{
	bool getAmmo = false;
	for (int i = 0; i < Character->Weapons.Num(); i++)
	{
		if (Character->Weapons[i]->AmmoType == AmmoType && Character->Weapons[i]->StockAmmo < Character->Weapons[i]->MaxStockAmmo)
		{
			getAmmo = true;

			//play sound ?

			Character->Weapons[i]->AddAmmo(Ammo);
			Character->AmmoNotif();
			Destroy();
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("AMMO %lld"), getAmmo));
	return getAmmo;
}

