// Copyright Epic Games, Inc. All Rights Reserved.

#include "Net/UnrealNetwork.h"
#include "UE_TPSCharacter.h"
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
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
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

	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Swap Weapon
	PlayerInputComponent->BindAction(FName("NextWeapon"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::NextWeapon);
	PlayerInputComponent->BindAction(FName("LastWeapon"), EInputEvent::IE_Pressed, this, &AUE_TPSCharacter::LastWeapon);

}

void AUE_TPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUE_TPSCharacter, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(AUE_TPSCharacter, CurrentWeapon, COND_None);
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

			CurrentWeapon->Mesh->SetVisibility(true);
			CurrentWeapon->CurrentOwner = this;
		}
	}
	if (OldWeapon)
	{
		OldWeapon->Mesh->SetVisibility(false);
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

void AUE_TPSCharacter::EquipWeapon(const int32 Index)
{
	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;

	if (IsLocallyControlled())
	{
		CurrentIndex = Index;

		const AWeapon* OldWeapon = CurrentWeapon;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(OldWeapon);
	}
	else if (!HasAuthority())
	{
		Server_SetCurrentWeapon(Weapons[Index]);
	}
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