// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UENUM(BlueprintType)
enum class AmmoType : uint8 {
	Rifle_Ammo			= 0		UMETA(DisplayName = "Rifle_Ammo"),
	Pistol_Ammo			= 1		UMETA(DisplayName = "Pistol_Ammo"),
	Shootgun_Ammo		= 2		UMETA(DisplayName = "Shootgun_Ammo"),
	Machinegun_Ammo		= 3		UMETA(DisplayName = "Machinegun_Ammo"),
	Sniper_Ammo			= 4		UMETA(DisplayName = "Sniper_Ammo"),
	RPG_Ammo			= 5		UMETA(DisplayName = "RPG_Ammo")
};

UCLASS()
class UE_TPS_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		TEnumAsByte<AmmoType> AmmoType = AmmoType::Rifle_Ammo;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sphere")
		UBoxComponent* activateBox;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	bool CollectAmmo(class AUE_TPSCharacter Character);

};
