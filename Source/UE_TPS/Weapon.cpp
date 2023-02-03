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
		Audios = { Audio0 };
	}
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

	//sound & animation
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
	


	// Acces a la fleche

	// Fleche Location
	FVector FlecheLocation = Arrow->GetComponentLocation();

	// Fleche Rotation (Direction)
	FRotator FlecheRotation = Arrow->GetComponentRotation();

	// Scale
	FVector ArrowScale = FVector(0.9f);

	///////////////////////////////////////////////////////////////////
	// Etincelles
	//  Affichage de la particle
	///////////////////////////////////////////////////////////////////

	// C'est une particle qui s'affiche, puis disparait � chaque fois
	// Contrairement � un feu/fum�e, par exemple
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sparks, FlecheLocation, FlecheRotation, ArrowScale);

	///////////////////////////////////////////////////////////////////
	// Audio du coup de feu
	///////////////////////////////////////////////////////////////////

	PlayAudio(Arrow, FlecheLocation, 0);

	//////////////////////////////////////////////////////////////////////
	// Direction du tir
	//////////////////////////////////////////////////////////////////////
	
	FVector Depart;
	FVector Fin;

	// Le rayon de Raycast va de "Depart" jusqu'� "Fin"
	// On doit recuperer les informations de "HIT" dans une structure.
	// Cette structure est le FHitResult

	FCollisionQueryParams InfoProperties;

	// Ignorer si le rayon atteint la propre classe (l'instance de l'arme)
	// Ignorer si le rayon atteint l'acteur qui tire
	InfoProperties.AddIgnoredActor(this);
	InfoProperties.AddIgnoredActor(Acteur);
	
	if (!ToTarget)
	{
		// Tirer par rapport � la direction du pistolet
		Depart = FlecheLocation;

		// Direction (Angle)
		FVector Direction = FlecheRotation.Vector();

		// Port�e du Tir
		Fin = Depart + (Direction * 3000.f);
	}
	else
	{
		// Tirer en direction de la cible, au centre de l'�cran
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

	// Tir
	// Afficher une ligne rouge montrant le parcours du tir effectu�
	// Diam�tre : 1.0f / Dur�e d'affichage : 5.0f secondes

	bool CibleAtteinte = GetWorld()->LineTraceSingleByChannel(HitResult, Depart, Fin, ECollisionChannel::ECC_Visibility, InfoProperties);

	if (CibleAtteinte)
	{
		// Quel acteur le TIR a atteint ?
		AActor* Acteur2 = HitResult.GetActor();

		// if (Acteur2 != nullptr)
		if (Acteur2)
		{
			FString ClassName = Acteur2->GetClass()->GetName();

			// Affichage de la particle : ImpactBullet
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBullet, HitResult.Location, HitResult.ImpactNormal.Rotation(), true);

			// Decal
			DrawBulletImpact();

			// Donner une impulsion physique (Impact physique)
			// (Ne pas confondre avec le lencer d'un projectile)
			// L'objet doit avoir "Simulate Physics" � TRUE, pour r�agir � l'impact physique de l'impulsion ou du projectile

			FString S = Acteur2->GetClass()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("S = %s"), *S);
			if (S == "StaticMeshActor")
			{
				UStaticMeshComponent* Comp = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());

				// Localisation de l'impulsion x Force d'impact
				if (Comp && Comp->IsSimulatingPhysics()) Comp->AddImpulse(HitResult.Location * 100.f);
			}

			if (S == "Target_C" || S == "TargetReverse_C" || S == "TargetVertical_C")
			{
				UTargetClass* Target = HitResult.GetActor()->FindComponentByClass<UTargetClass>();
				if (Target) Target->Hit();
			}
		}
	}
	
}

void AWeapon::DrawBulletImpact()
{
	ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(HitResult.Location, HitResult.ImpactNormal.Rotation());

	if (Decal)
	{
		// Assigner le material au Decal
		Decal->SetDecalMaterial(BulletDecal);

		// Limite pour voir le Decal au loin
		// Plus la valeur est petite, et plus on le voit de loin
		// Si la valeur est � 0, on le verra de toutes les distances
		Decal->GetDecal()->FadeScreenSize = 0.f;


		// Dur�e de vie du Decal
		// 0 = dur�e de vie illimit�e
		Decal->SetLifeSpan(0.0f);

		FVector Diametre = FVector(5.f);
		Decal->GetDecal()->DecalSize = Diametre;

		// Faire une rotation apr�s avoir spawn�, pour avoir le dessin correct
		Decal->SetActorRotation(HitResult.ImpactNormal.Rotation());
	}
}
