// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	//Arrow
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Mesh, FName("MuzzleFlash"));

	//Sparks
	SparksComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Etincelles"));
	SparksComp->bAutoActivate = false;
	SparksComp->SetupAttachment(Mesh, FName("MuzzleFlash"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> SparksObj(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (SparksObj.Object) Sparks = SparksObj.Object;
	SparksComp->SetTemplate(Sparks);

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactBloodObj(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Large_01.P_Impact_Metal_Large_01'"));
	if (ImpactBloodObj.Succeeded()) ImpactBlood = ImpactBloodObj.Object;
	
	ConstructorHelpers::FObjectFinder<UMaterial> BulletDecalObj(TEXT("Material'/Game/MilitaryWeapSilver/FX/Bonus/Weapon/M_Impact_Decal.M_Impact_Decal'"));
	if (BulletDecalObj.Succeeded()) BulletDecal = BulletDecalObj.Object;


	// Audios
	ConstructorHelpers::FObjectFinder<USoundWave> Audio0Obj(TEXT("SoundWave'/Game/MilitaryWeapSilver/Sound/Rifle/Wavs/RifleA_Fire_Loop_ST01.RifleA_Fire_Loop_ST01'"));
	if (Audio0Obj.Succeeded()) Audio0 = Audio0Obj.Object;
	Audios = { Audio0 };
}

//Play Audios
void AWeapon::PlayAudio(const UObject* Object, FVector Location, int Ref)
{
	if (Ref == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(Object, Audios[Ref], Location);
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(!CurrentOwner)
	Mesh->SetVisibility(false);
}
