// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_TPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon.h"

//////////////////////////////////////////////////////////////////////////
// AUE_TPSCharacter

AUE_TPSCharacter::AUE_TPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeLocation(FVector(0, 50.f, 90.f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetFieldOfView(110.f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	ConstructorHelpers::FObjectFinder<UAnimMontage> MontageShootingObj(TEXT("AnimMontage'/Game/Characters/Soldier/Animations/Soldier_Firing.Soldier_Firing'"));
	if (MontageShootingObj.Succeeded()) AM_Shooting = MontageShootingObj.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAimingObj(TEXT("AnimMontage'/Game/Characters/Soldier/Animations/Soldier_Riffle.Soldier_Riffle'"));
	if (MontageAimingObj.Succeeded()) AM_Aiming = MontageAimingObj.Object;


	ConstructorHelpers::FObjectFinder<UAnimMontage> MontageReloadingObj(TEXT("AnimMontage'/Game/Characters/Soldier/Animations/AM_Reloading.AM_Reloading'"));
	if (MontageReloadingObj.Succeeded()) AM_Reloading = MontageReloadingObj.Object;

	// Audios
	if (Audios.Num() <= 0)
	{
		USoundWave* Audio0 = NULL;
		ConstructorHelpers::FObjectFinder<USoundWave> Audio0Obj(TEXT("SoundWave'/Game/MilitaryWeapSilver/Sound/Rifle/Wavs/Rifle_Reload01.Rifle_Reload01'"));
		if (Audio0Obj.Succeeded()) Audio0 = Audio0Obj.Object;
		Audios = { Audio0 };
	}
}

void AUE_TPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HasAuthority())
	{
		for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons)
		{
			if (!WeaponClass) continue;
			FActorSpawnParameters Params;
			Params.Owner = this;
			AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params);
			const int32 Index = Weapons.Add(SpawnedWeapon);
			if (Index == CurrentIndex)
			{
				CurrentWeapon = SpawnedWeapon;
				OnRep_CurrentWeapon(nullptr);
			}
		}
	}

	ATPSGameMode* gm = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FString options = gm->OptionsString;

	if (options.Contains(FString("Easy")))
	{
		Minutes = 8;
	}
	else if (options.Contains(FString("Hard")))
	{
		Minutes = 3;
	}
	

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AUE_TPSCharacter::Countdown, 1.f, true, 0.0);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE_TPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE_TPSCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE_TPSCharacter::Look);
	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Swap Weapon
	PlayerInputComponent->BindAction(FName("NextWeapon"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::NextWeapon);
	PlayerInputComponent->BindAction(FName("LastWeapon"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::LastWeapon);

	//Aiming
	PlayerInputComponent->BindAction(FName("Aim"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::AimToTarget);

	//Shoot
	PlayerInputComponent->BindAction(FName("Shoot"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::StartShoot);
	PlayerInputComponent->BindAction(FName("Shoot"), EInputEvent::IE_Released, this, &AUE_TPSCharacter::StopShoot);

	//Drop Weapon
	PlayerInputComponent->BindAction(FName("DropWeapon"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::DropWeapon);

	//Pick
	PlayerInputComponent->BindAction(FName("Pick"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::Pick);

	//Reaload
	PlayerInputComponent->BindAction(FName("Reload"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::Reload);

	//Crounch
	PlayerInputComponent->BindAction(FName("Crounch"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::Crounch);

	PlayerInputC = PlayerInputComponent;

}

void AUE_TPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUE_TPSCharacter, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(AUE_TPSCharacter, CurrentWeapon, COND_None);
}

void AUE_TPSCharacter::WeaponNotif(bool active, FString txt)
{
	if (active)
	{
		notifTxt = txt;
	}
	else {
		notifTxt = "";
	}
	
}

void AUE_TPSCharacter::AmmoNotif(FString txt)
{
	notifTxt = txt;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AUE_TPSCharacter::EndAmmoNotif, 1.f, false, 3);
}

void AUE_TPSCharacter::EndAmmoNotif()
{
	notifTxt = "";
}

void AUE_TPSCharacter::Crounch()
{
	IsCrounch = !IsCrounch;
}

void AUE_TPSCharacter::OnRep_CurrentWeapon(const AWeapon* OldWeapon)
{
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->CurrentOwner)
		{
			const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("weaponsocket_r"));
			CurrentWeapon->SetActorTransform(GetMesh()->GetSocketTransform(FName("weaponsocket_r")), false, nullptr, ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("weaponsocket_r"));

			CurrentWeapon->CurrentOwner = this;
		}

		CurrentWeapon->Mesh->SetVisibility(true);
	}
	if (OldWeapon && !IsDrop)
	{
		OldWeapon->Mesh->SetVisibility(false);
	}
	else if (IsDrop) {
		IsDrop = false;
	}
}

void AUE_TPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUE_TPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//Play Audios
void AUE_TPSCharacter::PlayAudio(const UObject* Object, FVector Location, int Ref)
{
	if (Ref == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(Object, Audios[Ref], Location);
	}
}

//SHOOT
void AUE_TPSCharacter::StartShoot()
{
	bUseControllerRotationYaw = true;
	GetMesh()->GetAnimInstance()->Montage_Play(AM_Shooting, CurrentWeapon->FireRate);
	CurrentWeapon->StartShoot(this, true);
	IsShooting = true;
}

void AUE_TPSCharacter::StopShoot()
{
	bUseControllerRotationYaw = false;
	StopAMShooting();
	CurrentWeapon->StopShoot();
	IsShooting = false;
}

void AUE_TPSCharacter::StopAMShooting()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, AM_Shooting);
}

//WEAPONS
void AUE_TPSCharacter::EquipWeapon(const int32 Index)
{
	const AWeapon* OldWeapon = CurrentWeapon;

	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;

	if (IsLocallyControlled())
	{
		CurrentIndex = Index;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(OldWeapon);
	}
	else if (!HasAuthority())
	{
		Server_SetCurrentWeapon(Weapons[Index]);
	}
}

void AUE_TPSCharacter::DropWeapon()
{
	if (Weapons.Num() > 1) {

		CurrentWeapon->Drop();
		IsDrop = true;
		int i;
		for (i = 0; i < Weapons.Num(); i++)
		{
			if (Weapons[i] == CurrentWeapon)
			{
				Weapons.Remove(CurrentWeapon);
			}
		}
		AUE_TPSCharacter::NextWeapon();
	}
}

void AUE_TPSCharacter::Pick()
{
	TArray<AActor*> Result;
	GetOverlappingActors(Result, AWeapon::StaticClass());
	for (AActor* Actor : Result)
	{
		if (Actor->ActorHasTag("Pickupable"))
		{
			AWeapon* Weapon = Cast <AWeapon>(Actor);
			if (Weapon && !Weapon->CurrentOwner)
			{
				Weapon->Pick();
				const FTransform& PlacementTransform = Weapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("weaponsocket_r"));
				Weapon->SetActorTransform(PlacementTransform);
				Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("weaponsocket_r"));
				Weapon->CurrentOwner = this;

				Weapons.Add(Weapon);
				EquipWeapon(Weapons.Num() - 1);
			}
		}
	}

	GetOverlappingActors(Result, AAmmo::StaticClass());
	for (AActor* Actor : Result)
	{
		AAmmo* Ammo = Cast <AAmmo>(Actor);
		Ammo->CollectAmmo(this);
	}
}

void AUE_TPSCharacter::Reload()
{
	if (CurrentWeapon->StockAmmo > 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AM_Reloading, 2.0f);
		UGameplayStatics::PlaySoundAtLocation(this, Audios[0], this->GetActorLocation());
		CurrentWeapon->Reload();

		//Swap Weapon
		PlayerInputC->RemoveActionBinding(FName("NextWeapon"), EInputEvent::IE_Pressed);
		PlayerInputC->RemoveActionBinding(FName("LastWeapon"), EInputEvent::IE_Pressed);

		//Aiming
		PlayerInputC->RemoveActionBinding(FName("Aim"), EInputEvent::IE_Pressed);

		//Shoot
		PlayerInputC->RemoveActionBinding(FName("Shoot"), EInputEvent::IE_Pressed);
		PlayerInputC->RemoveActionBinding(FName("Shoot"), EInputEvent::IE_Released);

		//Drop Weapon
		PlayerInputC->RemoveActionBinding(FName("DropWeapon"), EInputEvent::IE_Pressed);

		//Pick
		PlayerInputC->RemoveActionBinding(FName("Pick"), EInputEvent::IE_Pressed);

		//Crounch
		PlayerInputC->RemoveActionBinding(FName("Crounch"), EInputEvent::IE_Pressed);
	}
}

void AUE_TPSCharacter::ReloadFinish()
{
	SetupPlayerInputComponent(PlayerInputC);
}

void AUE_TPSCharacter::Server_SetCurrentWeapon_Implementation(AWeapon* NewWeapon)
{
	const AWeapon* OldWeapon = CurrentWeapon;
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(OldWeapon);
}

void AUE_TPSCharacter::NextWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex + 1) ? CurrentIndex + 1 : 0;
	EquipWeapon(Index);
}

void AUE_TPSCharacter::LastWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex - 1) ? CurrentIndex - 1 : Weapons.Num() - 1;
	EquipWeapon(Index);
}

//AIM
void AUE_TPSCharacter::AimToTarget()
{	
	bUseControllerRotationYaw = true;
	// Switch aiming status
	bIsAiming = !bIsAiming;
	// Switch camera
	if (bIsAiming) {
		FollowCamera->SetRelativeLocation(FVector(0, 50.f, 90.f));
	}
	else {
		FollowCamera->SetRelativeLocation(FVector(300.f, 50.f, 90.f));
	}
	
	// Show crosshair in aiming mode
	if (bIsAiming)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AM_Aiming);
	}
	else 
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, AM_Aiming);
	}
}

void AUE_TPSCharacter::Countdown()
{
	if (Seconds != 0)
	{
		Seconds = Seconds - 1;
	}
	else
	{
		if (Minutes == 0)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "DefeatMenu");
		}
		else
		{
			Minutes = Minutes - 1;
			Seconds = 59;
		}
	}
}