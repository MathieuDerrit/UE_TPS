// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MYPROJECT2_API
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Ammo.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "TargetClass.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8 {
	Rifle_Weapon = 0		UMETA(DisplayName = "Rifle_Weapon"),
	Pistol_Weapon = 1		UMETA(DisplayName = "Pistol_Weapon"),
	ShootWeapon_Weapon = 2		UMETA(DisplayName = "ShootWeapon_Weapon"),
	MachineWeapon_Weapon = 3		UMETA(DisplayName = "MachineWeapon_Weapon"),
	Sniper_Weapon = 4		UMETA(DisplayName = "Sniper_Weapon"),
	RPG_Weapon = 5		UMETA(DisplayName = "RPG_Weapon")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	int MagazineAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	int StockAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	int MaxMagazineAmmo = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	int MaxStockAmmo = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TEnumAsByte<WeaponType> WeaponType = WeaponType::Rifle_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TEnumAsByte<AmmoType> AmmoType = AmmoType::Rifle_Ammo;

	//Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	float FireRate = 5.0f;

	FTimerHandle FullyAutomaticTimer;
	FTimerHandle FullyOneTimer;
	bool CanShoot = true;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sphere")
		USphereComponent* activateRadius;

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
	void Pick();
	void AddAmmo(int ammo);
	void Reload();

};
#undef MYPROJECT2_API
