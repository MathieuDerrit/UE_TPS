// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MYPROJECT2_API
#include "Components/ArrowComponent.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
		UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, Category = "Clef")
		UParticleSystemComponent* SparksComp;

	UParticleSystem* Sparks;
	UParticleSystem* ImpactBlood;
	UParticleSystem* ImpactBullet;

	UMaterialInterface* BulletDecal;

	//Audios
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XXX") USoundWave* Audio0;

	TArray<USoundWave*> Audios;

	void PlayAudio(const UObject* Object, FVector Location, int Ref);

};
#undef MYPROJECT2_API
