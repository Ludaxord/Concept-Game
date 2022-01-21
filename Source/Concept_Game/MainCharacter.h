// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "CoverPoint.h"
#include "CyberWeapon.h"
#include "FireWeapon.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "SkillComponent.h"
#include "MainCharacterInterface.h"
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
	ECM_TakeCover UMETA(DisplayName = "Take Cover Montage"),
	ECM_ExitCover UMETA(DisplayName = "Exit Cover Montage"),
	ECM_FixWeapon UMETA(DisplayName = "Fix Weapon Montage"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECameraState: uint8 {
	ECS_EyesCamera UMETA(DisplayName = "Eyes Camera"),
	ECS_FollowCamera UMETA(DisplayName = "Follow Camera"),
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
class CONCEPT_GAME_API AMainCharacter : public ACharacter, public IMainCharacterInterface {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetTestWeapon();

public:
	void SetLookUpRates(float DeltaTime);
	void CalculateCrosshairSpread(float DeltaTime);
	// void TraceForItems();
	// void TraceForLadder();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ConstructCharacterMovement() const;

	void ConstructPostProcess();

	void SetDefaultCameras();

	//TODO: Move systems like covers, using weapons, using environment and jumping/parkour to different class to make it usable for AI Character and Player Character
	void Cover();
	void PeakLeft();
	void PeakRight();
	void PeakTop();

	bool GetForwardTracers(FVector& OutStart, FVector& OutEnd, FHitResult& TracerHitResult);
	bool GetInCoverMouseTracer(FVector& OutStart, FVector& OutEnd);

	bool IsWeaponUsable();
	void PlayCharacterSound(ECharacterSoundState CharacterSoundState);
	void PerformAttack();
	void StartCrosshairMovement();
	void StartAttackTimer(EWeaponType WeaponType);
	void UseWeaponByType(EWeaponType WeaponType);
	void UseWeapon();

	virtual bool TraceForLevelChange(FHitResult& OutHitResult, FVector& OutHitLocation);
	virtual bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void Jump() override;
	void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	void Crouching();
	void Crawling();
	void Standing();
	void Climbing();

	void Aim();

	void StopAiming();

	void AimingFieldOfView(float DeltaTime);

	void StartFireTimer();

	void InterpCapsuleHalfHeight(float DeltaTime);

	//TODO: In Target only change camera to Follow Camera when player stick to cover,
	//TODO: NOTE: Button need to be pressed to stick to cover, when only close to cover player will lean from cover. 
	//TODO: NOTE: When stick to cover mode is on, there is no need to press aiming, only analog/mouse movement will give ability to aim. Like Deus Ex Aiming mechanic.
	// void ChangeDebugCamera();
	// void ChangeDebugTriggerRotationYaw();
	// void TriggerFPSCounter();

	void SwitchCamera(bool bFollowCamera);

	UFUNCTION()
	void AutoFireReset();

	void ConstructCameraBoom();
	void ConstructFollowCamera();
	void ConstructEyesCamera();
	void ConstructRefFollowCamera();
	void ConstructRefFollowCameraArrowComponent();
	void ConstructCoverArrows();
	void ConstructEyesCameraHeadComponent();
	void FinishCrosshairMovement();

	void PlayMontage(ECharacterMontage CharacterMontage, EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	void GrapClip();

	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	AWeapon* SpawnDefaultWeapon(EWeaponType WeaponType = EWeaponType::EWT_Any);

	template <typename T>
	T* SpawnWeapon(TSubclassOf<T> WeaponClass);

	template <typename T>
	T* SpawnCoverPoint(TSubclassOf<T> CoverPortalClass);

	bool RemoveCoverPoint(ACoverPoint* Point);

	void EquipWeapon(AWeapon* WeaponToEquip, FName SocketName = "RightHandSocket", bool bSwapping = false);

	void DropItem(AItem* ItemToDrop);

	void SwapWeapon(AWeapon* WeaponToSwap);

	void SphereOverlapBegin(FGuid Guid);
	void SphereOverlapEnd(FGuid Guid);

	void CoverSystem();

	void EnterCover();

	void ExitCover();

	void LeftTracer();

	void RightTracer();

	void TopTracer();

	void HideCoverOnCameraTrace();

	void MoveRightInCover(float Value);

	void TraceCharacterCover();

	void TraceCoverDisable();

	void TraceCoverDisableWhileInCover();

	bool FrontTraceCoverDisable();

	bool LeftTraceCoverDisable();

	bool RightTraceCoverDisable();

	bool CrosshairTraceCoverDisable();

	bool LeftTraceCoverJumpBetweenCovers();

	bool RightTraceCoverJumpBetweenCovers();

	bool CoverTracer(UArrowComponent* AComponent, FHitResult& Result, float HalfHeight = 60.0f,
	                 ETraceTypeQuery TraceType = ETraceTypeQuery::TraceTypeQuery1,
	                 FLinearColor TraceColor = FLinearColor::Red, FLinearColor HitColor = FLinearColor::Green);

	bool TraceCoverMovement(float Orientation);

	void InCoverMoving();

	void MoveInCover();

	UFUNCTION(BlueprintCallable)
	void MoveBetweenCovers();

	UFUNCTION()
	void CoverMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool GetPlayerHUDVisibility() const;

	bool GetPauseMenuVisibility() const;

protected:
	FTransform SetCameraTransform(class UCameraComponent* Camera, FName SocketName = "",
	                              bool AttackComponent = false, USkeletalMeshComponent* Parent = nullptr) const;
	void SetActiveCameras(bool FollowCameraActive);

	UFUNCTION()
	void OnCameraTimelineFloatUpdate(float Output);

	UFUNCTION()
	void OnCameraTimelineFinished();

private:
	friend class UMainCharacterInputComponent;

	UFUNCTION()
	void UpdateClimbingTransitionTimeline(float Output);

	UFUNCTION()
	void UpdateAimTransitionTimeline(float Output);

	UFUNCTION()
	FVector MoveToLocation() const;

	virtual void CanCover_Implementation(bool bCover) override;
	virtual void MoveLeftRight_Implementation(float Direction) override;
	virtual void PeakLeft_Implementation(bool PeakLeft) override;
	virtual void PeakRight_Implementation(bool PeakLeft) override;
	virtual void PeakTop_Implementation(bool PeakTop) override;

	//TODO: Create inventory class to store informations like guids...
	TArray<FGuid> ItemGuids;

	bool bRotationYaw;

	bool bFPSCounter;

	float InitialRightHandYaw;

	bool bPauseMenuButtonPressed;

	//TODO: Move to CoverComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	FVector CoverLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	FVector CoverNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bInCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverDisable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverLeftDisable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverRightDisable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverMontageEnded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoveringActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverPeakLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverPeakRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCoverPeakTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveInCover;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* TakeCoverMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CoverLeftMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CoverRightMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CoverDisableLeftMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CoverDisableRightMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* CoverTopMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveRightDirectionTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveLeftDirectionTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCanPeakTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bSlideToLeftCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bSlideToRightCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACover* SlideLeftCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACover* SlideRightCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCanPeakLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bCanPeakRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMouseRightForwardMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMouseLeftForwardMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveLeftLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveRightLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveForwardLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveForwardRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bMoveForwardDisable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	FVector TempCharacterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	FVector RefFollowCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	FHitResult CurrentCoverHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	class ACover* CurrentCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionResponse> CurrentCoverCollisionResponse;

	bool bCanCover;
	bool bCameraMoved;
	bool bUseXInCoverMovement;

	bool bStoreTolerance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bTraceCoverLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	bool bTraceCoverRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACoverPoint> DefaultCoverPointClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACoverPoint* CurrentCoverPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACoverPoint* CoverPointEndTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	float CoverToCoverAnimPlayRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACoverPoint* LeftCoverPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover", meta = (AllowPrivateAccess = "true"))
	ACoverPoint* RightCoverPoint;

	FVector CurrentCoverOrigin;

	FVector CurrentCoverBoxExtend;

	float Tolerance;

	AActor* CoverLeftSphereBlocker;

	AActor* CoverRightSphereBlocker;

	UPROPERTY(EditAnywhere, Category="Camera")
	UCurveFloat* ClimbingTransitionFloatCurve;

	FOnTimelineFloat UpdateClimbingFunctionFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ClimbingTransitionTimeline;

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
	USkeletalMeshComponent* EyesCameraHeadComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	FTransform EyesCameraTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera", meta = (AllowPrivateAccess = "true"))
	FTransform FollowCameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bEnableCameraTransition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	ECameraState CameraState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairSpreadMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairSpreadMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairHalfWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D ScreenCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D CrossHairLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D CrossHairBaseCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ChangeCameraTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* AimTransitionTimeline;

	UPROPERTY(EditAnywhere, Category="Aim Properties")
	UCurveFloat* AimTransitionFloatCurve;

	FOnTimelineFloat AimUpdateFunctionFloat;

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

	float CoverPitch;
	float CoverRoll;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
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

	bool bDebugFollowCamera;

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

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	// class AItem* TraceHitItem;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	// class AItem* CurrentInteractItem;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	// AItem* TraceHitItemLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	EPoseType PoseType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bOverlappingLadderBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bOverlappingLadderTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	int TimesJumped;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveRightValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float LookValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	float LookUpVal;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Aim Properties", meta = (AllowPrivateAccess = "true"))
	float CurrentAimValue;

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

	float ClimbRightValue;

	bool bPlayClimbTurnAnimation;
	bool bClimbingButtonPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UCoverComponent* CharacterCoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fog", meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* FogComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UShootingComponent* CharacterShootingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* CharacterInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class USkillComponent* CharacterSkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UBaseHUDComponent* CharacterBaseHUDComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* CharacterMeleeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UParkourComponent* CharacterParkourComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UEnhancementComponent* CharacterEnhancementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UItemComponent* CharacterItemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UPauseMenuComponent* CharacterPauseMenuComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UMainCharacterCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UMainCharacterInputComponent* CharacterInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* CharacterQuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UDebugWidgetComponent* DebugWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UDialogComponent* CharacterDialogComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UStealthComponent* CharacterStealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class ULockpickComponent* CharacterLockpickComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UHackingComponent* CharacterHackingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UDoorComponent* CharacterDoorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UFlashlightComponent* CharacterFlashLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UMapComponent* CharacterMapComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class URenderingComponent* CharacterRenderingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* InvisibleCharacterMesh;
public:
	TArray<FGuid> OverlappedItemIDs;

	bool bJumpFromClimb;

	bool bTouchingFloor;

	USkeletalMeshComponent* GetInvisibleCharacterMesh() {
		return InvisibleCharacterMesh;
	}

	UCoverComponent* GetCharacterCoverComponent() {
		return CharacterCoverComponent;
	};

	UShootingComponent* GetCharacterShootingComponent() {
		return CharacterShootingComponent;
	};

	UBaseHUDComponent* GetCharacterBaseHUDComponent() {
		return CharacterBaseHUDComponent;
	};

	UMeleeComponent* GetCharacterMeleeComponent() {
		return CharacterMeleeComponent;
	};

	UParkourComponent* GetCharacterParkourComponent() {
		return CharacterParkourComponent;
	};

	UEnhancementComponent* GetCharacterEnhancementComponent() {
		return CharacterEnhancementComponent;
	};

	UInventoryComponent* GetCharacterInventoryComponent() {
		return CharacterInventoryComponent;
	};

	UItemComponent* GetCharacterItemComponent() {
		return CharacterItemComponent;
	};

	UMainCharacterCameraComponent* GetCharacterCameraComponent() {
		return CharacterCameraComponent;
	};

	UMainCharacterInputComponent* GetCharacterInputComponent() {
		return CharacterInputComponent;
	};

	URenderingComponent* GetCharacterRenderingComponent() {
		return CharacterRenderingComponent;
	};

	FORCEINLINE USpringArmComponent* GetCameraBoom() const {
		return CameraBoom;
	}

	FORCEINLINE bool GetJumpFromClimb() const {
		return bJumpFromClimb;
	}

	FORCEINLINE float GetCurrentAimValue() const {
		return CurrentAimValue;
	}

	FORCEINLINE ECameraState GetCameraState() const {
		return CameraState;
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

	FORCEINLINE bool GetInCover() const {
		return bInCover;
	}

	FORCEINLINE bool GetSlideToRightCover() const {
		return bSlideToRightCover;
	}

	FORCEINLINE bool GetSlideToLeftCover() const {
		return bSlideToLeftCover;
	}

	FORCEINLINE bool GetIsMoveLeft() const {
		return bMoveLeft;
	}

	FORCEINLINE bool GetIsMoveRight() const {
		return bMoveRight;
	}

	FORCEINLINE bool GetTraceCoverLeft() const {
		return bTraceCoverLeft;
	}

	FORCEINLINE bool GetTraceCoverRight() const {
		return bTraceCoverRight;
	}

	FORCEINLINE bool GetMouseLeftForwardMove() const {
		return bMouseLeftForwardMove;
	}

	FORCEINLINE bool GetMouseRightForwardMove() const {
		return bMouseRightForwardMove;
	}

	FORCEINLINE bool GetCoverMontageEnded() const {
		return bCoverMontageEnded;
	}

	FORCEINLINE bool GetCoveringActive() const {
		return bCoveringActive;
	}

	FORCEINLINE bool GetAimingButtonPressed() const {
		return bAimingButtonPressed;
	}

	FORCEINLINE EPoseType GetCurrentPoseType() const {
		return PoseType;
	}

	FORCEINLINE float GetMoveRightValue() const {
		return MoveRightValue;
	}

	FORCEINLINE float GetTurnValue() const {
		return TurnValue;
	}

	FORCEINLINE float GetLookValue() const {
		return LookValue;
	}

	FORCEINLINE void SetTraceCoverLeft(bool InTraceCoverLeft) {
		bTraceCoverLeft = InTraceCoverLeft;
	}

	FORCEINLINE void SetTraceCoverRight(bool InTraceCoverRight) {
		bTraceCoverRight = InTraceCoverRight;
	}

	FORCEINLINE void SetCoverPeakLeft(bool InCoverPeakLeft) {
		bCoverPeakLeft = InCoverPeakLeft;
	}

	FORCEINLINE void SetCoverPeakRight(bool InCoverPeakRight) {
		bCoverPeakRight = InCoverPeakRight;
	}

	FORCEINLINE void SetCoverPeakTop(bool InCoverPeakTop) {
		bCoverPeakTop = InCoverPeakTop;
	}

	FORCEINLINE void SetSlideToRightCover(bool InSlideToRightCover) {
		bSlideToRightCover = InSlideToRightCover;
	}

	FORCEINLINE void SetSlideToLeftCover(bool InSlideToLeftCover) {
		bSlideToLeftCover = InSlideToLeftCover;
	}

	FORCEINLINE ACover* GetCurrentCover() {
		return CurrentCover;
	}

	FORCEINLINE bool GetCanPeakLeft() {
		return bCanPeakLeft;
	}

	FORCEINLINE float GetCoverToCoverAnimPlayRate() {
		return CoverToCoverAnimPlayRate;
	}

	FORCEINLINE bool GetCanPeakRight() {
		return bCanPeakRight;
	}

	FORCEINLINE bool GetCanPeakTop() {
		return bCanPeakTop;
	}

	FORCEINLINE bool GetCoverPeakLeft() {
		return bCoverPeakLeft;
	}

	FORCEINLINE bool GetCoverPeakRight() {
		return bCoverPeakRight;
	}

	FORCEINLINE bool GetCoverPeakTop() {
		return bCoverPeakTop;
	}

	FORCEINLINE bool GetMoveLeft() {
		return bMoveLeft;
	}

	FORCEINLINE bool GetMoveRight() {
		return bMoveRight;
	}

	FORCEINLINE bool GetMoveInCover() {
		return bMoveInCover;
	}

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE bool GetRunning() const {
		return bRunning;
	}

	FORCEINLINE bool GetOverlappingLadderBottom() const {
		return bOverlappingLadderBottom;
	}

	FORCEINLINE bool GetOverlappingLadderTop() const {
		return bOverlappingLadderTop;
	}

	FORCEINLINE ECombatState GetCombatState() const {
		return CombatState;
	}

	FORCEINLINE AWeapon* GetEquippedWeapon() const {
		return EquippedWeapon;
	}

	FORCEINLINE AItem* GetCurrentInteractItem() const {
		return CharacterItemComponent->GetCurrentInteractItem();
	}

	void SetEquippedWeapon(AWeapon* InNewWeapon) {
		EquippedWeapon = InNewWeapon;
	}

	void SetOverlappingLadderTop(bool OverlappingLadderTop) {
		bOverlappingLadderTop = OverlappingLadderTop;
	}

	void SetOverlappingLadderBottom(bool OverlappingLadderBottom) {
		bOverlappingLadderBottom = OverlappingLadderBottom;
	}

	void SetCurrentInteractItem(AItem* InCurrentInteractItem) {
		CharacterItemComponent->SetCurrentInteractItem(InCurrentInteractItem);
	}

	void SetPoseType(EPoseType InPoseType) {
		PoseType = InPoseType;
	}
};
