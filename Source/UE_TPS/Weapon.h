// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MYPROJECT2_API
#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FIKProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimSequence* AnimPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AimOffset = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform CustomOffsetTransform;
};

/**
 *
 */
UCLASS(Abstract)

class MYPROJECT2_API AWeapon : public AActor
{
	GENERATED_BODY()
public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		class AUE_TPSCharacter* CurrentOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
		FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
		FTransform PlacementTransform;

};
#undef MYPROJECT2_API
