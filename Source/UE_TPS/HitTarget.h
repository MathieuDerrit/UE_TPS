// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitTarget.generated.h"
#include "GameFramework/Actor.h"

class UBoxComponent;

UCLASS()

class UE_TPS_API UHitTarget : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* HitBox;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent^* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
		
};
