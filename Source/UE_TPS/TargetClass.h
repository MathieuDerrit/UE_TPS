// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameMode.h"
#include "TargetClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_TPS_API UTargetClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere)
		bool isReverse;

	UPROPERTY(EditAnywhere)
		bool isVertical;

	UPROPERTY(EditAnywhere)
		FRotator CurrentRotation;

	UPROPERTY(EditAnywhere)
		FRotator HitRotation;	
	
	UPROPERTY(EditAnywhere)
		FRotator HitRotationReversed;

	UPROPERTY(EditAnywhere)
		FRotator HitRotationVertical;

	UPROPERTY(EditAnywhere)
		bool isHit = false;

	UFUNCTION()
		void Hit();
	
	UFUNCTION(BlueprintCallable)
		void Scoring();
};