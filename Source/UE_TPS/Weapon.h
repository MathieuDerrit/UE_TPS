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
#include "TargetClass.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		class AActor* Acteur;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		bool ToTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		bool IsVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
		FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
		FTransform PlacementTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* SparksComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* Sparks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ImpactBullet;

	//Audios
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<USoundWave*> Audios;

	//Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	float FireRate = 5.0f;

	FTimerHandle FullyAutomaticTimer;
	FTimerHandle FullyOneTimer;
	bool CanShoot = true;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
		UArrowComponent* Arrow;

	UMaterialInterface* BulletDecal;

	void PlayAudio(const UObject* Object, FVector Location, int Ref);

	void DrawBulletImpact();

	FHitResult HitResult;

public:
	void Shoot();
	void StartShoot(AActor* Acteur, bool ToTarget);
	void StopShoot();
	void SetShootReady();
	void Drop();
	//AWeapon Pick();

};
#undef MYPROJECT2_API
