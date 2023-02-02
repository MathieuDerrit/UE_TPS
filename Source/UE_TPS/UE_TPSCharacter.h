// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Weapon.h"
#include "Ammo.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "UE_TPSCharacter.generated.h"


UCLASS(config=Game)
class AUE_TPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool IsShooting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool IsDrop = false;

	//Audios
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TArray<USoundWave*> Audios;

public:
	AUE_TPSCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PlayAudio(const UObject* Object, FVector Location, int Ref);

	virtual void NextWeapon();
	virtual void LastWeapon();
	virtual void AimToTarget();
	virtual void StartShoot();
	virtual void StopShoot();
	virtual void Reload();

	bool bIsAiming = false;
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Spawn weapon default
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TArray<TSubclassOf<class AWeapon>> DefaultWeapons;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UInputComponent* PlayerInputC;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
	TArray<class AWeapon*> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
	class AWeapon* CurrentWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	int32 CurrentIndex = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FString notifTxt = "";

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void EquipWeapon(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void DropWeapon();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void Pick();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void ReloadFinish();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AM_Shooting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AM_Aiming;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AM_Reloading;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AmmoNotif(FString txt);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void EndAmmoNotif();


protected: 
	UFUNCTION()
	virtual void OnRep_CurrentWeapon(const class AWeapon* OldWeapon);

	UFUNCTION(Server, Reliable)
	void Server_SetCurrentWeapon(class AWeapon* Weapon);
	virtual void Server_SetCurrentWeapon_Implementation(class AWeapon* Weapon);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void StopAMShooting();
};

