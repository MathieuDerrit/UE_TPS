//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "TPSGameMode.h"
//#include "Kismet/GameplayStatics.h"
//#include "DestroyBarrier.generated.h"
//
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class UE_TPS_API ADestroyBarrier : public AActor
//{
//	GENERATED_BODY()
//
//public:	
//	// Sets default values for this component's properties
//	ADestroyBarrier();
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
//		class USceneComponent* Root;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
//		class UStaticMeshComponent* Mesh;
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//
//	UFUNCTION()
//		void DestroyObject();
//};
