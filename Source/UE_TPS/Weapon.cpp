// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "UE_TPSCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	Tags = { "Pickupable" };

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	//Create activate trigger radius
	activateRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Activate Radius"));
	activateRadius->InitSphereRadius(100.0f);
	activateRadius->SetSphereRadius(100.0f);
	activateRadius->SetGenerateOverlapEvents(true);
	activateRadius->SetupAttachment(Mesh, FName("AmmoEject"));
	activateRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	activateRadius->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);
	activateRadius->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnOverlapEnd);
	activateRadius->bHiddenInGame = true;
	activateRadius->SetVisibility(false);

	//Arrow
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Mesh, FName("MuzzleFlash"));

	//Sparks
	SparksComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Etincelles"));
	SparksComp->bAutoActivate = false;
	SparksComp->SetupAttachment(Mesh, FName("MuzzleFlash"));

	if (!Sparks)
	{
		ConstructorHelpers::FObjectFinder<UParticleSystem> SparksObj(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
		if (SparksObj.Object) Sparks = SparksObj.Object;
		SparksComp->SetTemplate(Sparks);
	}
	if (!ImpactBullet)
	{
		ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactBulletObj(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Medium_01.P_Impact_Metal_Medium_01'"));
		if (ImpactBulletObj.Succeeded()) ImpactBullet = ImpactBulletObj.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UMaterial> BulletDecalObj(TEXT("Material'/Game/Effects/Materials/Weapon/M_Impact_Decal.M_Impact_Decal'"));
	if (BulletDecalObj.Succeeded()) BulletDecal = BulletDecalObj.Object;


	// Audios
	if (Audios.Num() <= 0) 
	{
		USoundWave* Audio0 = NULL;
		ConstructorHelpers::FObjectFinder<USoundWave> Audio0Obj(TEXT("SoundWave'/Game/MilitaryWeapSilver/Sound/Rifle/Wavs/RifleA_Fire_End_ST02.RifleA_Fire_End_ST02'"));
		if (Audio0Obj.Succeeded()) Audio0 = Audio0Obj.Object;
		Audios.Push(Audio0);
	}

	USoundWave* Audio1 = NULL;
	ConstructorHelpers::FObjectFinder<USoundWave> Audio1Obj(TEXT("SoundWave'/Game/MilitaryWeapSilver/Sound/Knife/Wavs/Knife_Raise.Knife_Raise'"));
	if (Audio1Obj.Succeeded()) Audio1 = Audio1Obj.Object;
	AddAmmoAudio = Audio1;
	

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
	if (!CurrentOwner && !IsVisible)
	{
		Mesh->SetVisibility(false);
		activateRadius->SetSphereRadius(0.0f);
	}
	
}

void AWeapon::Drop()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CurrentOwner = nullptr;
	activateRadius->SetSphereRadius(100.0f);
}

void AWeapon::Pick()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->ResetRelativeTransform();
	activateRadius->SetSphereRadius(0.0f);
}

void AWeapon::AddAmmo(int ammo)
{
	UGameplayStatics::PlaySoundAtLocation(this, AddAmmoAudio, this->GetActorLocation());
	StockAmmo += ammo;
	if (StockAmmo > MaxStockAmmo)
	{
		StockAmmo = MaxStockAmmo;
	}
}

void AWeapon::Reload()
{
	int amooNeed = MaxMagazineAmmo - MagazineAmmo;
	if (StockAmmo >= amooNeed) 
	{ 
		MagazineAmmo += amooNeed;
		StockAmmo -= amooNeed; 
	}
	else
	{ 
		MagazineAmmo += StockAmmo;
		StockAmmo = 0;
	}
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUE_TPSCharacter* Player = Cast <AUE_TPSCharacter>(OtherActor);
	if (Player) {
		//FString txt = FString("Press F to take ") + FString::FromInt(Ammo) + FString(UEnum::GetValueAsString(Character->Weapons[i]->AmmoType.GetValue())).Replace(*FString("AmmoType::"), *FString(""), ESearchCase::CaseSensitive).Replace(*FString("_Ammo"), *FString(" Ammo"), ESearchCase::CaseSensitive);
		FString txt = FString("Press F to take ") + GetClass()->GetName().Replace(*FString("BP_"), *FString(""), ESearchCase::CaseSensitive).Replace(*FString("_C"), *FString(""), ESearchCase::CaseSensitive);
		Player->WeaponNotif(true, txt);
	}

}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AUE_TPSCharacter* Player = Cast <AUE_TPSCharacter>(OtherActor);
	if (Player) {
		Player->WeaponNotif(false);
	}

}

void AWeapon::SetShootReady()
{
	CanShoot = true;
}

//SHOOT
void AWeapon::StartShoot(AActor* Actor, bool Target)
{
	Acteur = Actor;
	ToTarget = Target;
	UWorld* const World = GetWorld();
	if (CanShoot)
	{
		Shoot();
		CanShoot = false;
		World->GetTimerManager().SetTimer(FullyOneTimer, this, &AWeapon::SetShootReady, 1.0f / FireRate, false);
	}
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(FullyAutomaticTimer, this, &AWeapon::Shoot, 1.0f / FireRate, true);
	}

}

void AWeapon::StopShoot()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(FullyAutomaticTimer);
	}
}

void AWeapon::Shoot()
{

	FString TireurClassName = Acteur->GetClass()->GetName();
	AUE_TPSCharacter* Player = Cast <AUE_TPSCharacter>(Acteur);

	if (Player->CurrentWeapon->MagazineAmmo <= 0)
	{
		Player->StopAMShooting();
		return;
	}
	else {
		Player->CurrentWeapon->MagazineAmmo--;
	}
	
	//Fleche de tir
	FVector FlecheLocation = Arrow->GetComponentLocation();
	FRotator FlecheRotation = Arrow->GetComponentRotation();
	FVector ArrowScale = FVector(0.9f);

	//FX tir
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sparks, FlecheLocation, FlecheRotation, ArrowScale);

	// Audio tir
	PlayAudio(Arrow, FlecheLocation, 0);


	// Direction tir
	FVector Depart;
	FVector Fin;
	FCollisionQueryParams InfoProperties;

	InfoProperties.AddIgnoredActor(this);
	InfoProperties.AddIgnoredActor(Acteur);
	
	if (!ToTarget)
	{
		Depart = FlecheLocation;
		FVector Direction = FlecheRotation.Vector();
		Fin = Depart + (Direction * 3000.f);
	}
	else
	{
		FVector2D ViewportSize;

		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		FVector2D TargetLocation = FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector TargetWorldPosition;
		FVector TargetWorldDirection;

		bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), TargetLocation, TargetWorldPosition, TargetWorldDirection);

		if (ScreenToWorld)
		{
			Depart = TargetWorldPosition;
			Fin = TargetWorldPosition + TargetWorldDirection * 50000.f;
		}

	}

	//Pour un tir réaliste (Réduire la précision du raycast)
	Fin.X += FMath::RandRange(-500, 500);
	Fin.Y += FMath::RandRange(-500, 500);
	Fin.Z += FMath::RandRange(-500, 500);

	bool CibleAtteinte = GetWorld()->LineTraceSingleByChannel(HitResult, Depart, Fin, ECollisionChannel::ECC_Visibility, InfoProperties);

	if (CibleAtteinte)
	{
		AActor* Acteur2 = HitResult.GetActor();

		if (Acteur2)
		{
			FString ClassName = Acteur2->GetClass()->GetName();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBullet, HitResult.Location, HitResult.ImpactNormal.Rotation(), true);
			DrawBulletImpact();
			FString S = Acteur2->GetClass()->GetName();

			//SI la cible est un static mesh
			if (S == "StaticMeshActor")
			{
				UStaticMeshComponent* Comp = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());

				// Localisation de l'impulsion x Force d'impact
				if (Comp && Comp->IsSimulatingPhysics()) Comp->AddImpulse(HitResult.Location * 100.f);
			}

			//SI la cible est une cible
			if (S == "Target_C" || S == "TargetReverse_C" || S == "TargetVertical_C")
			{
				UTargetClass* Target = HitResult.GetActor()->FindComponentByClass<UTargetClass>();
				if (Target) Target->Hit();
			}
		}
	}
	
}

//Traces de balles
void AWeapon::DrawBulletImpact()
{
	ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(HitResult.Location, HitResult.ImpactNormal.Rotation());

	if (Decal)
	{
		Decal->SetDecalMaterial(BulletDecal);
		Decal->GetDecal()->FadeScreenSize = 0.f;
		Decal->SetLifeSpan(0.0f);
		FVector Diametre = FVector(5.f);
		Decal->GetDecal()->DecalSize = Diametre;
		Decal->SetActorRotation(HitResult.ImpactNormal.Rotation());
	}
}
