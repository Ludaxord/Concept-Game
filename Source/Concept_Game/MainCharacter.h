// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "CyberWeapon.h"
#include "FireWeapon.h"
#include "MeleeWeapon.h"
#include "PoseType.h"
#include "ThrowableWeapon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8 {
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "Fire Timer In Progress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Equipping UMETA(DisplayName = "Equipping"),
	ECS_Stunned UMETA(DisplayName = "Stunned"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECharacterSoundState : uint8 {
	ECSS_UseWeapon UMETA(DisplayName = "Use Weapon Sound"),
	ECSS_ReloadWeapon UMETA(DisplayName = "Reload Weapon Sound"),
	ECSS_FixWeapon UMETA(DisplayName = "Fix Weapon Sound"),
	ECSS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECharacterMontage: uint8 {
	ECM_UseWeapon UMETA(DisplayName = "Use Weapon Montage"),
	ECM_ReloadWeapon UMETA(DisplayName = "Reload Weapon Montage"),
	ECM_FixWeapon UMETA(DisplayName = "Fix Weapon Montage"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	int32 ItemCount;
};


UCLASS()
class CONCEPT_GAME_API AMainCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetLookUpRates(float DeltaTime);
	void CalculateCrosshairSpread(float DeltaTime);
	void TraceForItems();
	void TraceForLadder();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ConstructCharacterMovement() const;

	void SetDefaultCameras();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ClimbRight(float Value);
	void TurnRate(float Rate);
	void Turn(float Value);
	void LookUpAtRate(float Rate);
	void LookUp(float Value);

	bool IsWeaponUsable();
	void PlayCharacterSound(ECharacterSoundState CharacterSoundState);
	void PerformAttack();
	void StartCrosshairMovement();
	void StartAttackTimer(EWeaponType WeaponType);
	void UseWeaponByType(EWeaponType WeaponType);
	void UseWeapon();

	virtual bool TraceForLevelChange(FHitResult& OutHitResult, FVector& OutHitLocation);
	virtual bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void AimingButtonPressed();
	void AimingButtonReleased();

	void RunningButtonPressed(float Value);
	void RunningButtonReleased();

	void ChangePoseButtonPressed(FKey Key);
	void ChangePoseAxisButtonPressed(float Value);

	void Jump() override;

	void Crouching();
	void Crawling();
	void Standing();
	void Climbing();

	void Aim();

	void StopAiming();

	void AimingFieldOfView(float DeltaTime);

	void UseWeaponButtonPressed();
	void UseWeaponButtonReleased();

	void StartFireTimer();

	void InterpCapsuleHalfHeight(float DeltaTime);

	//TODO: In Target only change camera to Follow Camera when player stick to cover,
	//TODO: NOTE: Button need to be pressed to stick to cover, when only close to cover player will lean from cover. 
	//TODO: NOTE: When stick to cover mode is on, there is no need to press aiming, only analog/mouse movement will give ability to aim. Like Deus Ex Aiming mechanic.
	void ChangeDebugCamera();
	void ChangeDebugTriggerRotationYaw();
	void TriggerFPSCounter();

	void SwitchCamera(bool bFollowCamera);

	UFUNCTION()
	void AutoFireReset();

	void ConstructCameraBoom();
	void ConstructFollowCamera();
	void ConstructEyesCamera();
	void ConstructRefFollowCamera();
	void ConstructRefFollowCameraArrowComponent();
	void FinishCrosshairMovement();

	void PlayMontage(ECharacterMontage CharacterMontage, EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	void GrapClip();

	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	AWeapon* SpawnDefaultWeapon(EWeaponType WeaponType = EWeaponType::EWT_Any);

	template <typename T>
	T* SpawnWeapon(TSubclassOf<T> WeaponClass);

	void EquipWeapon(AWeapon* WeaponToEquip, FName SocketName = "RightHandSocket", bool bSwapping = false);

	void DropItem(AItem* ItemToDrop);

	void SwapWeapon(AWeapon* WeaponToSwap);

	void InteractButtonPressed();

	void InventoryButtonPressed();

	void PauseButtonPressed();

	void QuickSelectButtonPressed(float Value);
	void SphereOverlapBegin(FGuid Guid);
	void SphereOverlapEnd(FGuid Guid);

protected:
	FTransform SetCameraTransform(class UCameraComponent* Camera, FName SocketName = "",
	                              bool AttackComponent = false, USkeletalMeshComponent* Parent = nullptr) const;
	void SetActiveCameras(bool FollowCameraActive) const;

	UFUNCTION()
	void OnCameraTimelineFloatUpdate(float Output);

	UFUNCTION()
	void OnCameraTimelineFinished();

private:
	//TODO: Create inventory class to store informations like guids...
	TArray<FGuid> ItemGuids;

	bool bRotationYaw;

	bool bFPSCounter;

	TArray<FGuid> OverlappedItemIDs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* EyesCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* RefFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* RefFollowCameraRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	FTransform EyesCameraTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	FTransform FollowCameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bEnableCameraTransition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ChangeCameraTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ChangeCameraFloatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat UpdateCameraTimelineFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	FOnTimelineEvent UpdateCameraTimelineEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	FOnTimelineEvent FinishCameraTimelineEvent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	bool bSwitchToFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ForceMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpolationSpeed;

	bool bUseWeaponButtonPressed;

	bool bAimingButtonPressed;

	bool bShouldAttack;

	FTimerHandle AutoFireTimer;

	FTimerHandle CrosshairShootTimer;

	float ShootFireDuration;

	bool bFiringBullet;

	bool bShouldTraceForItems;

	float PoseAxisValueCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraZoomedFOV;

	float CameraCurrentFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFireWeapon> DefaultFireWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACyberWeapon> DefaultCyberWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AThrowableWeapon> DefaultThrowableWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeleeWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* CurrentInteractItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	EPoseType PoseType;

	EPoseType LastPoseType;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	// bool bCrouching;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	// bool bCrawling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	bool bRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Items", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float RunningMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float CrawlingMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float AimingMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float CrawlingGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stealth", meta = (AllowPrivateAccess = "true"))
	float CrouchCharacterVisibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stealth", meta = (AllowPrivateAccess = "true"))
	float RunningCharacterVisibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stealth", meta = (AllowPrivateAccess = "true"))
	float CrawlingCharacterVisibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stealth", meta = (AllowPrivateAccess = "true"))
	float BaseCharacterVisibility;

	float CurrentCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float CrawlingCapsuleHalfHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Items", meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items", meta= (AllowPrivateAccess = "true"))
	int32 InventoryCapacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* DefaultCrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* DefaultCrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* DefaultCrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* DefaultCrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* DefaultCrosshairsTop;

	float ClimbStartRotationYaw;

	float ClimbStartRotationYawLeft;
	float ClimbStartRotationYawRight;
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {
		return CameraBoom;
	}

	FORCEINLINE UCameraComponent* GetFollowCamera() const {
		return FollowCamera;
	}

	FORCEINLINE UCameraComponent* GetEyesCamera() const {
		return EyesCamera;
	}

	FORCEINLINE UCameraComponent* GetRefFollowCamera() const {
		return RefFollowCamera;
	}

	FORCEINLINE bool GetAiming() const {
		return bAiming;
	}

	FORCEINLINE EPoseType GetCurrentPoseType() const {
		return PoseType;
	}

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE bool GetRunning() const {
		return bRunning;
	}

	FORCEINLINE ECombatState GetCombatState() const {
		return CombatState;
	}

	FORCEINLINE AWeapon* GetEquippedWeapon() const {
		return EquippedWeapon;
	}

	FORCEINLINE AItem* GetCurrentInteractItem() const {
		return CurrentInteractItem;
	}

	void SetEquippedWeapon(AWeapon* InNewWeapon) {
		EquippedWeapon = InNewWeapon;
	}

	void SetCurrentInteractItem(AItem* InCurrentInteractItem) {
		CurrentInteractItem = InCurrentInteractItem;
	}

	void SetPoseType(EPoseType InPoseType) {
		PoseType = InPoseType;
	}
};
