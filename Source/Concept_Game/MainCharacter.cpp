// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "DrawDebugHelpers.h"
#include "MainAnimInstance.h"
#include "MainHUD.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Sound/SoundCue.h"

// Sets default values
AMainCharacter::AMainCharacter():
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	HipTurnRate(90.0f),
	HipLookUpRate(90.0f),
	AimingTurnRate(20.0f),
	AimingLookUpRate(20.0f),
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f),
	bAiming(false),
	bOverlappingLadderBottom(false),
	bOverlappingLadderTop(false),
	bJumpFromClimb(false),
	bPlayClimbTurnAnimation(false),
	bUseWeaponButtonPressed(false),
	bShouldTraceForItems(false),
	bRotationYaw(false),
	bAimingButtonPressed(false),
	CrouchGroundFriction(100.0f),
	CrawlingGroundFriction(50.0f),
	ZoomInterpolationSpeed(20.0f),
	BaseGroundFriction(2.0f),
	bShouldAttack(false),
	bCoverActive(false),
	CameraDefaultFOV(0.0f),
	CameraZoomedFOV(35.0f),
	CameraCurrentFOV(0.0f),
	bRunning(false),
	bSwitchToFollowCamera(false),
	bEnableCameraTransition(false),
	bFPSCounter(false),
	CombatState(ECombatState::ECS_Unoccupied),
	CrosshairSpreadMultiplier(0.0f),
	CrosshairVelocityFactor(0.0f),
	CrosshairInAirFactor(0.0f),
	CrosshairShootingFactor(0.0f),
	CrouchMovementSpeed(300.0f),
	RunningMovementSpeed(650.0f),
	PoseAxisValueCounter(0),
	CrawlingMovementSpeed(250.0f),
	AimingMovementSpeed(350.0f),
	BaseMovementSpeed(350.0f),
	CrouchCharacterVisibility(50.0f),
	RunningCharacterVisibility(100.0f),
	CrawlingCharacterVisibility(30.0f),
	BaseCharacterVisibility(80.0f),
	StandingCapsuleHalfHeight(88.0f),
	CrouchingCapsuleHalfHeight(44.0f),
	CrawlingCapsuleHalfHeight(22.0f),
	PoseType(EPoseType::EPT_Stand),
	LastPoseType(EPoseType::EPT_Stand),
	Health(100.0f),
	MaxHealth(100.0f) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructCameraBoom();
	ConstructFollowCamera();
	ConstructEyesCamera();
	ConstructRefFollowCamera();
	ConstructRefFollowCameraArrowComponent();
	ConstructCoverArrows();

	AimTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTransitionTimeline"));

	ChangeCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ChangeCameraTimeline"));

	ClimbingTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ClimbingTransitionTimeline"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	ConstructCharacterMovement();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	SetDefaultCameras();

	//TODO: Add Weapon and inventory (Depend on game progress)
	EquipWeapon(SpawnDefaultWeapon());
	EquippedWeapon->SetSlotIndexX(0);
	EquippedWeapon->SetSlotIndexY(0);
	EquippedWeapon->SetCharacter(this);
	//TODO: Add Ammo (Depend on game progress)

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	UpdateClimbingFunctionFloat.BindDynamic(this, &AMainCharacter::UpdateClimbingTransitionTimeline);
	if (ClimbingTransitionFloatCurve) {
		ClimbingTransitionTimeline->AddInterpFloat(ClimbingTransitionFloatCurve, UpdateClimbingFunctionFloat);
	}

	AimUpdateFunctionFloat.BindDynamic(this, &AMainCharacter::UpdateAimTransitionTimeline);
	if (AimTransitionFloatCurve) {
		AimTransitionTimeline->AddInterpFloat(AimTransitionFloatCurve, AimUpdateFunctionFloat);
		UE_LOG(LogTemp, Warning, TEXT("Setting Aim Transition"));
	}

	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMainCharacter::CoverMontageEnded);

	InCoverMoving();
}

void AMainCharacter::SetLookUpRates(float DeltaTime) {
	BaseTurnRate = bAiming ? AimingTurnRate : HipTurnRate;
	BaseLookUpRate = bAiming ? AimingLookUpRate : HipLookUpRate;
}

void AMainCharacter::CalculateCrosshairSpread(float DeltaTime) {
	FVector2D WalkSpeedRange = {0.0f, 600.0f};
	FVector2D VelocityMultiplierRange = {0.0f, 1.0f};
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange,
	                                                            Velocity.Size());
	CrosshairInAirFactor = GetCharacterMovement()->IsFalling()
		                       ? FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f)
		                       : FMath::FInterpTo(CrosshairInAirFactor, 0.0f, DeltaTime, 30.0f);

	CrosshairAimFactor = bAiming
		                     ? FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.0f)
		                     : FMath::FInterpTo(CrosshairAimFactor, 0.0f, DeltaTime, 30.0f);

	CrosshairShootingFactor = bFiringBullet
		                          ? FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.0f)
		                          : FMath::FInterpTo(CrosshairShootingFactor, 0.0f, DeltaTime, 60.0f);

	CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor +
		CrosshairShootingFactor;
}

void AMainCharacter::TraceForItems() {

	// if (bShouldTraceForItems) {
	if (OverlappedItemIDs.Num() > 0) {
		FHitResult ItemTraceHitResult;
		FVector HitLocation;
		TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
		if (ItemTraceHitResult.bBlockingHit) {
			TraceHitItem = Cast<AItem>(ItemTraceHitResult.Actor);

			//TODO: If Trace hit item exists, switch between item types...
			if (TraceHitItem) {
				// UE_LOG(LogTemp, Error, TEXT("Tracing item: %s"), *TraceHitItem->GetName());
			}

			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterp) {
				TraceHitItem = nullptr;
			}

			if (TraceHitItem && TraceHitItem->GetPickupWidget()) {
				TraceHitItem->PerformTrace(this, OverlappedItemIDs);
			}

			if (TraceHitItemLastFrame) {
				if (TraceHitItem != TraceHitItemLastFrame) {
					TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
				}
			}

			TraceHitItemLastFrame = TraceHitItem;
		}
		else if (TraceHitItemLastFrame) {
			TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
		}
	}
	else if (TraceHitItemLastFrame) {
		TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
	}

}

void AMainCharacter::TraceForLadder() {
	FHitResult LadderTraceHitResult;
	FVector HitLocation;
	bool Trace = TraceForLevelChange(LadderTraceHitResult, HitLocation);
	if (PoseType == EPoseType::EPT_Climb) {
		bJumpFromClimb = !Trace;
		bTouchingFloor = Trace;
	}
}

bool AMainCharacter::TraceForLevelChange(FHitResult& OutHitResult, FVector& OutHitLocation) {
	const FVector Start = GetActorLocation();
	const FVector End = GetActorUpVector() * (-100.0f) + GetActorLocation();
	OutHitLocation = End;
	GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 50.0f);
	if (OutHitResult.bBlockingHit) {
		OutHitLocation = OutHitResult.Location;
		return true;
	}

	return false;
}


bool AMainCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation) {

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation = {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
	CrosshairLocation.Y -= 50.0f;

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;


	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld) {
		const FVector Start = CrosshairWorldPosition;
		const FVector End = Start + CrosshairWorldDirection * 50000.0f;
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);

		if (OutHitResult.bBlockingHit) {
			// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 50.0f);
			// DrawDebugPoint(GetWorld(), OutHitResult.Location, 15.0f, FColor::Cyan, false, 50.0f);
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AimingFieldOfView(DeltaTime);
	SetLookUpRates(DeltaTime);
	CalculateCrosshairSpread(DeltaTime);
	TraceForItems();
	TraceForLadder();
	InterpCapsuleHalfHeight(DeltaTime);

	CoverSystem();

	if (bInCover) {
		UE_LOG(LogTemp, Warning, TEXT("MoveRight: %s, MoveLeft: %s"), bMoveRight ? TEXT("true"): TEXT("false"),
		       bMoveLeft ? TEXT("true") : TEXT("false"));
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ClimbRight", this, &AMainCharacter::ClimbRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookupRate", this, &AMainCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("PoseChange", this, &AMainCharacter::ChangePoseAxisButtonPressed);
	PlayerInputComponent->BindAxis("Running", this, &AMainCharacter::RunningButtonPressed);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("QuickSelect", this, &AMainCharacter::QuickSelectButtonPressed);

	//NOTE, Debug bindings, remove in production
	PlayerInputComponent->BindAction("DebugChangeCamera", IE_Pressed, this, &AMainCharacter::ChangeDebugCamera);
	PlayerInputComponent->BindAction("FPSCounterTrigger", IE_Pressed, this, &AMainCharacter::TriggerFPSCounter);
	PlayerInputComponent->BindAction("DebugTriggerRotationYaw", IE_Pressed, this,
	                                 &AMainCharacter::ChangeDebugTriggerRotationYaw);


	PlayerInputComponent->BindAction("PoseChange", IE_Pressed, this, &AMainCharacter::ChangePoseButtonPressed);
	PlayerInputComponent->BindAction("ClimbRightAction", IE_Pressed, this, &AMainCharacter::ClimbRightActionPressed);
	PlayerInputComponent->BindAction("ClimbRightAction", IE_Released, this, &AMainCharacter::ClimbRightActionReleased);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &AMainCharacter::Cover);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &AMainCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &AMainCharacter::AimingButtonReleased);
	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed, this, &AMainCharacter::UseWeaponButtonPressed);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this, &AMainCharacter::UseWeaponButtonReleased);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::InteractButtonPressed);
	PlayerInputComponent->BindAction("DebugDropItem", IE_Pressed, this, &AMainCharacter::DebugDropItem);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMainCharacter::InventoryButtonPressed);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::PauseButtonPressed);

}

void AMainCharacter::ConstructCharacterMovement() const {
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void AMainCharacter::SetDefaultCameras() {
	ChangeCameraTimeline->SetTimelineLength(0.1f);

	if (bEnableCameraTransition)
		UpdateCameraTimelineFloat.BindDynamic(this, &AMainCharacter::OnCameraTimelineFloatUpdate);
	FinishCameraTimelineEvent.BindDynamic(this, &AMainCharacter::OnCameraTimelineFinished);

	ChangeCameraTimeline->AddInterpFloat(ChangeCameraFloatCurve, UpdateCameraTimelineFloat);
	// ChangeCameraTimeline->SetTimelinePostUpdateFunc(UpdateCameraTimelineEvent);
	ChangeCameraTimeline->SetTimelineFinishedFunc(FinishCameraTimelineEvent);
	ChangeCameraTimeline->RegisterComponent();

	if (FollowCamera) {
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
		FollowCamera->SetActive(false);
	}

	if (EyesCamera) {
		CameraState = ECameraState::ECS_EyesCamera;
		EyesCamera->SetActive(true);
	}

	if (RefFollowCamera) {
		RefFollowCamera->SetActive(false);
	}
}

void AMainCharacter::MoveForward(float Value) {
	if (Controller != nullptr && (Value != 0.0f)) {
		if (bInCover) {
			UE_LOG(LogTemp, Warning, TEXT("TurnValue: %f, LookValue: %f"), TurnVal, LookUpVal, CoverLeftMovement)
		}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = {0, Rotation.Yaw, 0};
		FVector Direction;
		if (PoseType == EPoseType::EPT_Climb) {
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			if (Value < 0.0f) {
				if (GetOverlappingLadderBottom() && bTouchingFloor) {
					PoseType = EPoseType::EPT_Stand;
					GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
					SwitchCamera(false);
				}
			}
			else if (Value > 0.0f) {
				if (GetOverlappingLadderTop()) {

				}
			}
		}
		else {
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value) {
	MoveRightValue = Value;
	if (bInCover) {
		bMoveLeft = Value > 0.0f;
		bMoveRight = Value < 0.0f;
	}
	if (Controller != nullptr && (Value != 0.0f)) {
		if (PoseType != EPoseType::EPT_Climb) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation = {0, Rotation.Yaw, 0};
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
			ClimbStartRotationYaw = Controller->GetControlRotation().Yaw;
			ClimbStartRotationYawLeft = ClimbStartRotationYaw + 45.0f;
			ClimbStartRotationYawRight = ClimbStartRotationYaw - 45.0f;
		}
	}

}

void AMainCharacter::ClimbRight(float Value) {
	if (Controller != nullptr) {
		if (PoseType == EPoseType::EPT_Climb) {
			ClimbRightValue = Value;
			if (bClimbingButtonPressed) {
				if (bPlayClimbTurnAnimation)
					ClimbingTransitionTimeline->PlayFromStart();
			}
			else {
				if (bPlayClimbTurnAnimation)
					ClimbingTransitionTimeline->PlayFromStart();
			}
		}
	}

}

void AMainCharacter::ClimbRightActionPressed() {
	bPlayClimbTurnAnimation = true;
	bClimbingButtonPressed = true;
}

void AMainCharacter::ClimbRightActionReleased() {
	bPlayClimbTurnAnimation = true;
	bClimbingButtonPressed = false;
}

void AMainCharacter::TurnRate(float Rate) {
	// if (PoseType != EPoseType::EPT_Climb)
	TurnValue = Rate;
	// if (bInCover || bCoverActive || bCoveringActive) return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Turn(float Value) {
	if (PoseType == EPoseType::EPT_Climb) return;
	TurnVal = Value;
	if (bInCover || bCoverActive || bCoveringActive) return;
	float TurnScaleFactor = bAiming ? MouseAimingTurnRate : MouseHipTurnRate;
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AMainCharacter::LookUpAtRate(float Rate) {
	// if (PoseType != EPoseType::EPT_Climb)
	LookValue = Rate;
	// if (bInCover || bCoverActive || bCoveringActive) return;
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUp(float Value) {
	if (PoseType == EPoseType::EPT_Climb) return;
	LookUpVal = Value;
	if (bInCover || bCoverActive || bCoveringActive) return;
	float LookUpScaleFactor = bAiming ? MouseAimingLookUpRate : MouseHipLookUpRate;
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AMainCharacter::Cover() {
	// if (bCanCover) {
	if (!bCoverActive) {
		FVector OutStart;
		FVector OutEnd;
		GetForwardTracers(OutStart, OutEnd);

		TArray<AActor*> IgnoredActors;
		FHitResult OutHitResult;
		bool bTraced = UKismetSystemLibrary::SphereTraceSingle(this,
		                                                       OutStart,
		                                                       OutEnd,
		                                                       80.0f,
		                                                       ETraceTypeQuery::TraceTypeQuery1,
		                                                       false,
		                                                       IgnoredActors,
		                                                       EDrawDebugTrace::Type::ForOneFrame,
		                                                       OutHitResult,
		                                                       true
		);
		if (bTraced) {
			UE_LOG(LogTemp, Warning, TEXT("bTraced: %s, OutStart: %s, OutEnd: %s"),
			       bTraced ? TEXT("true") : TEXT("false"), *OutStart.ToString(), *OutEnd.ToString());
			if (OutHitResult.bBlockingHit) {
				CoverLocation = OutHitResult.Location;
				CoverNormal = OutHitResult.Normal;
				EnterCover();
				//TODO: Add anim montage And rotation Root Yaw.
				PlayMontage(ECharacterMontage::ECM_TakeCover,
				            EquippedWeapon != nullptr
					            ? EquippedWeapon->GetWeaponType()
					            : EWeaponType::EWT_Any);
				bCoveringActive = true;
			}
		}
	}
	else {
		if (bInCover) {
			ExitCover();
			bCoveringActive = false;
			// PlayMontage(ECharacterMontage::ECM_ExitCover);
			//TODO: Add anim montage And rotation Root Yaw.
		}
	}
	bCoverActive = !bCoverActive;
	// }
	// else {
	// 	bCoverActive = false;
	// }
	UE_LOG(LogTemp, Warning, TEXT("CoverActive: %s InCover: %s CanCover: %s"),
	       bCoverActive ? TEXT("true") : TEXT("false"),
	       bInCover ? TEXT("true") : TEXT("false"),
	       bCanCover ? TEXT("true") : TEXT("false"));
}

bool AMainCharacter::GetForwardTracers(FVector& OutStart, FVector& OutEnd) {
	FVector RotFVector = GetActorRotation().Quaternion().GetForwardVector();
	FVector MultipliedFVector = {RotFVector.X * 80.0f, RotFVector.Y * 80.0f, RotFVector.Z};
	float CoverRadius = 20.0f;
	OutStart = GetActorLocation();
	OutEnd = OutStart + MultipliedFVector;

	TArray<AActor*> IgnoredActors;
	FHitResult OutHitResult;
	return UKismetSystemLibrary::SphereTraceSingle(this,
	                                               OutStart,
	                                               OutEnd,
	                                               CoverRadius,
	                                               ETraceTypeQuery::TraceTypeQuery1,
	                                               false,
	                                               IgnoredActors,
	                                               EDrawDebugTrace::Type::ForOneFrame,
	                                               OutHitResult,
	                                               true
	);
}

bool AMainCharacter::IsWeaponUsable() {
	return EquippedWeapon == nullptr ? false : EquippedWeapon->GetUsability();
}

void AMainCharacter::PlayCharacterSound(ECharacterSoundState CharacterSoundState) {
	switch (CharacterSoundState) {
	case ECharacterSoundState::ECSS_UseWeapon:
		if (EquippedWeapon->GetUsageSound()) {
			UE_LOG(LogTemp, Warning, TEXT("PlayCharacterSound ECSS_UseWeapon"));
			UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetUsageSound());
		}
		break;
	case ECharacterSoundState::ECSS_ReloadWeapon: break;
	case ECharacterSoundState::ECSS_FixWeapon: break;
	case ECharacterSoundState::ECSS_MAX: break;
	default: break;
	}
}

void AMainCharacter::PerformAttack() {
	EquippedWeapon->PerformAttack();
}

void AMainCharacter::StartCrosshairMovement() {
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(CrosshairShootTimer, this, &AMainCharacter::FinishCrosshairMovement,
	                                ShootFireDuration);
}

void AMainCharacter::StartAttackTimer(EWeaponType WeaponType) {
}

void AMainCharacter::UseWeaponByType(EWeaponType WeaponType) {
	const TEnumAsByte<EWeaponType> WeaponEnum = WeaponType;
	FString EnumAsString = UEnum::GetValueAsString(WeaponEnum.GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Weapon Type: %s"), *EnumAsString);

	switch (WeaponType) {
	case EWeaponType::EWT_Melee: {
	}
	break;
	case EWeaponType::EWT_Fire: {
		PlayCharacterSound(ECharacterSoundState::ECSS_UseWeapon);
		PerformAttack();
		PlayMontage(ECharacterMontage::ECM_UseWeapon, EquippedWeapon->GetWeaponType());
		StartCrosshairMovement();
		EquippedWeapon->DecreaseUsability();
		StartAttackTimer(EquippedWeapon->GetWeaponType());
		EquippedWeapon->StartWeaponAnimationTimer();
	}
	break;
	case EWeaponType::EWT_Force: {
	}
	break;
	case EWeaponType::EWT_Any: break;
	case EWeaponType::EWT_Throwable: break;
	case EWeaponType::EWT_MAX: break;
	default: ;
	}
}

void AMainCharacter::ConstructCameraBoom() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.0f);
}

void AMainCharacter::ConstructFollowCamera() {
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetActive(false);
}

void AMainCharacter::ConstructEyesCamera() {
	EyesCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EyesCamera"));
	EyesCamera->SetupAttachment(GetMesh(), "head");
	EyesCamera->bUsePawnControlRotation = true;
	EyesCamera->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
	ConstructEyesCameraHeadComponent();
}

void AMainCharacter::ConstructRefFollowCamera() {
	RefFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RefFollowCamera"));
	RefFollowCamera->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::ConstructRefFollowCameraArrowComponent() {
	RefFollowCameraRotation = CreateDefaultSubobject<UArrowComponent>(TEXT("RefFollowCameraRotation"));
	RefFollowCameraRotation->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::ConstructCoverArrows() {
	CoverLeftMovement = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverLeftMovement"));
	CoverLeftMovement->SetupAttachment(GetCapsuleComponent());
	CoverRightMovement = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverRightMovement"));
	CoverRightMovement->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::ConstructEyesCameraHeadComponent() {
	EyesCameraHeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyesCameraHeadComponent"));
	EyesCameraHeadComponent->SetupAttachment(GetMesh(), "head");
	EyesCameraHeadComponent->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
	EyesCameraHeadComponent->SetOwnerNoSee(true);
	EyesCameraHeadComponent->bCastHiddenShadow = true;
	// EyesCameraHeadComponent->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::FinishCrosshairMovement() {
	bFiringBullet = false;
}

void AMainCharacter::UseWeapon() {
	if (Health <= 0.0f) return;
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (PoseType == EPoseType::EPT_Climb) return;
	if (!IsWeaponUsable()) return;

	UE_LOG(LogTemp, Warning, TEXT("Use Weapon"));

	UseWeaponByType(EquippedWeapon->GetWeaponType());

}

void AMainCharacter::AimingButtonPressed() {
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	CrossHairSpreadMulti = HUD->GetCrossHairSpreadMultiplier();
	CrossHairSpreadMax = HUD->GetCrossHairSpreadMax();
	CrossHairHalfHeight = HUD->GetCrossHairHalfHeight();
	CrossHairHalfWidth = HUD->GetCrossHairHalfWidth();
	ScreenCenter = HUD->GetScreenCenter();
	CrossHairLocation = HUD->GetCrossHairLocation();
	CrossHairBaseCenter = HUD->GetCrossHairBaseCenter();

	bAimingButtonPressed = true;
	if (CombatState != ECombatState::ECS_Reloading && CombatState != ECombatState::ECS_Equipping && CombatState !=
		ECombatState::ECS_Stunned)
		Aim();
}

void AMainCharacter::AimingButtonReleased() {
	bAimingButtonPressed = false;
	StopAiming();
}

void AMainCharacter::RunningButtonPressed(float Value) {
	bRunning = (PoseType == EPoseType::EPT_Stand && Value > 0);
	// UE_LOG(LogTemp, Warning, TEXT("Running %s"), bRunning ? TEXT("true") : TEXT("false"))
}

void AMainCharacter::RunningButtonReleased() {
	if (PoseType == EPoseType::EPT_Stand)
		bRunning = false;
}

void AMainCharacter::ChangePoseButtonPressed(FKey Key) {
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	auto KeyTime = Player->GetInputKeyTimeDown(Key);


	switch (PoseType) {
	case EPoseType::EPT_Stand:
		Crouching();
		break;
	case EPoseType::EPT_Crouch: {
		Standing();
	}
	break;
	case EPoseType::EPT_Crawl:
		Crouching();
		break;
	case EPoseType::EPT_Run:
		break;
	case EPoseType::EPT_Aim:
		break;
	case EPoseType::EPT_Climb:
		Climbing();
		break;
	case EPoseType::EPT_MAX: break;
	default:
		Standing();
		break;
	}

	const TEnumAsByte<EPoseType> PoseEnum = PoseType;
	FString EnumAsString = UEnum::GetValueAsString(PoseEnum.GetValue());

	UE_LOG(LogTemp, Warning, TEXT("Pose: %s, Pressed Key %s, Key Time %f Key Value"), *EnumAsString,
	       *(Key.GetDisplayName().ToString()), KeyTime)
}

void AMainCharacter::ChangePoseAxisButtonPressed(float Value) {
	if (PoseAxisValueCounter > 80) {
		Crawling();
	}

	if (Value == 0)
		PoseAxisValueCounter = 0;
	else
		PoseAxisValueCounter += Value;

}

void AMainCharacter::Jump() {
	if (PoseType != EPoseType::EPT_Climb) {
		if (PoseType != EPoseType::EPT_Stand) {
			UE_LOG(LogTemp, Warning, TEXT("Jumping Not Stand"));
			PoseType = EPoseType::EPT_Stand;
			GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Jumping Base"));
			Super::Jump();
		}
	}
	else {
		bJumpFromClimb = true;
		PoseType = EPoseType::EPT_Stand;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		SwitchCamera(false);
		UE_LOG(LogTemp, Warning, TEXT("Jumping From Climb %s"), bJumpFromClimb ? TEXT("true") : TEXT("false"));
		Super::Jump();
	}

}

void AMainCharacter::Crouching() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Crouch;

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrouchGroundFriction;
}

void AMainCharacter::Crawling() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Crawl;

	GetCharacterMovement()->MaxWalkSpeed = CrawlingMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrawlingGroundFriction;
	LastPoseType = PoseType;
}

void AMainCharacter::Standing() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Stand;

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	LastPoseType = PoseType;
}

void AMainCharacter::Climbing() {
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	LastPoseType = PoseType;
}

void AMainCharacter::Aim() {
	AimTransitionTimeline->PlayFromStart();
}

void AMainCharacter::StopAiming() {
	AimTransitionTimeline->ReverseFromEnd();
}

void AMainCharacter::AimingFieldOfView(float DeltaTime) {
	if (bSwitchToFollowCamera) {
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, bAiming ? CameraZoomedFOV : CameraDefaultFOV, DeltaTime,
		                                    ZoomInterpolationSpeed);
		GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
	}
	else {
		//TODO: Create aiming in EyesCamera FOV
	}

}

void AMainCharacter::UseWeaponButtonPressed() {
	bUseWeaponButtonPressed = true;
	UseWeapon();
}

void AMainCharacter::UseWeaponButtonReleased() {
	bUseWeaponButtonPressed = false;
}

void AMainCharacter::StartFireTimer() {
}

void AMainCharacter::InterpCapsuleHalfHeight(float DeltaTime) {
	float TargetCapsuleHalfHeight = 0;
	float InterpSpeed = 20.0f;
	switch (PoseType) {

	case EPoseType::EPT_Stand:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Crouch:
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Run:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Aim:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Crawl:
		TargetCapsuleHalfHeight = 34.0f;
		InterpSpeed = 1.f;
		break;
	case EPoseType::EPT_Climb:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_MAX: break;
	default: ;
	}

	const float InterpHalfHeight = FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	                                                TargetCapsuleHalfHeight,
	                                                DeltaTime,
	                                                InterpSpeed);

	const float DeltaCapsuleHalfHeight = InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector MeshOffset = {0.0f, 0.0f, -DeltaCapsuleHalfHeight};
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AMainCharacter::ChangeDebugCamera() {

	if (!GetCharacterMovement()->IsFalling()) {
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		EyesCameraTransform = SetCameraTransform(EyesCamera, "head", true, GetMesh());
		bSwitchToFollowCamera = !bSwitchToFollowCamera;
		UE_LOG(LogTemp, Error, TEXT("Camera TPP: %s"), bSwitchToFollowCamera ? TEXT("true") : TEXT("false"))

		if (!bSwitchToFollowCamera) {
			FollowCameraTransform = SetCameraTransform(FollowCamera);
			ChangeCameraTimeline->PlayFromStart();

			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = true;
			bUseControllerRotationRoll = false;
		}
		else {
			SetActiveCameras(true);
			FollowCameraTransform = RefFollowCamera->GetComponentTransform();
			ChangeCameraTimeline->ReverseFromEnd();
			//TODO: Fix animation when change controller rotation Yaw to false;
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = bRotationYaw;
			bUseControllerRotationRoll = false;

		}
	}
}

void AMainCharacter::ChangeDebugTriggerRotationYaw() {
	bRotationYaw = !bRotationYaw;
	UE_LOG(LogTemp, Warning, TEXT("Change Debug Trigger Rotation Yaw %s"), bRotationYaw ? TEXT("true") : TEXT("false"));
}

void AMainCharacter::TriggerFPSCounter() {
	bFPSCounter = !bFPSCounter;
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "stat fps");
}

void AMainCharacter::SwitchCamera(bool bFollowCamera) {
	if (bSwitchToFollowCamera != bFollowCamera) {
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		EyesCameraTransform = SetCameraTransform(EyesCamera, "head", true, GetMesh());
		bSwitchToFollowCamera = bFollowCamera;
		UE_LOG(LogTemp, Error, TEXT("Camera TPP: %s"), bSwitchToFollowCamera ? TEXT("true") : TEXT("false"))

		if (!bSwitchToFollowCamera) {
			FollowCameraTransform = SetCameraTransform(FollowCamera);
			ChangeCameraTimeline->PlayFromStart();

			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = true;
			bUseControllerRotationRoll = false;
		}
		else {
			SetActiveCameras(true);
			FollowCameraTransform = RefFollowCamera->GetComponentTransform();
			ChangeCameraTimeline->ReverseFromEnd();
			//TODO: Fix animation when change controller rotation Yaw to false;
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = bRotationYaw;
			bUseControllerRotationRoll = false;
		}
	}
}

void AMainCharacter::AutoFireReset() {
}

void AMainCharacter::PlayMontage(ECharacterMontage CharacterMontage, EWeaponType WeaponType) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	const TEnumAsByte<EWeaponType> WeaponEnum = WeaponType;
	FString WeaponEnumAsString = UEnum::GetValueAsString(WeaponEnum.GetValue());
	const TEnumAsByte<ECharacterMontage> MontageEnum = CharacterMontage;
	FString MontageEnumAsString = UEnum::GetValueAsString(MontageEnum.GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Montage Weapon Type: %s, Character Montage: %s"), *WeaponEnumAsString,
	       *MontageEnumAsString);
	if (AnimInstance) {
		switch (CharacterMontage) {
		case ECharacterMontage::ECM_UseWeapon: {
			switch (WeaponType) {
			case EWeaponType::EWT_Melee: {
				if (MeleeWeaponMontage) {
					AnimInstance->Montage_Play(MeleeWeaponMontage);
					AnimInstance->Montage_JumpToSection(FName("StartMelee"));
				}
			}
			break;
			case EWeaponType::EWT_Fire: {
				if (FireWeaponMontage) {
					AnimInstance->Montage_Play(FireWeaponMontage);
					AnimInstance->Montage_JumpToSection(FName("StartFire"));
					// UE_LOG(LogTemp, Warning, TEXT("Hip Fire Montage Run"))
				}
			}
			break;
			case EWeaponType::EWT_Force: {
				if (ForceMontage) {
					AnimInstance->Montage_Play(ForceMontage);
					AnimInstance->Montage_JumpToSection(FName("StartCyber"));
				}
			}
			break;
			case EWeaponType::EWT_Any: break;
			case EWeaponType::EWT_Throwable: break;
			case EWeaponType::EWT_MAX: break;
			default: ;
			}
		}
		break;
		case ECharacterMontage::ECM_ReloadWeapon: break;
		case ECharacterMontage::ECM_FixWeapon: break;
		case ECharacterMontage::ECM_TakeCover: {
			//TODO: Fix to weapon types
			const FName SectionName = WeaponType == EWeaponType::EWT_Fire
				                          ? FName("StartCover")
				                          : FName("StartCoverTwoHandWeapon");
			if (AnimInstance && TakeCoverMontage) {
				UE_LOG(LogTemp, Warning, TEXT("Section Name Montage Play: %s"), *SectionName.ToString())
				bCoverMontageEnded = false;
				AnimInstance->Montage_Play(TakeCoverMontage);
				AnimInstance->Montage_JumpToSection(SectionName);
			}
		}
		break;
		case ECharacterMontage::ECM_ExitCover: {
		}
		break;
		case ECharacterMontage::ECS_MAX: break;
		default: ;
		}
	}
}

void AMainCharacter::GrapClip() {
}

void AMainCharacter::ReleaseClip() {
}

AWeapon* AMainCharacter::SpawnDefaultWeapon(EWeaponType WeaponType) {
	AWeapon* WeaponCreated = nullptr;
	switch (WeaponType) {
	case EWeaponType::EWT_Any: {
		if (DefaultFireWeaponClass) {
			WeaponCreated = SpawnWeapon(DefaultFireWeaponClass);
		}

		if (!WeaponCreated) {
			if (DefaultCyberWeaponClass) {
				WeaponCreated = SpawnWeapon(DefaultCyberWeaponClass);
			}
		}

		if (!WeaponCreated) {
			if (DefaultMeleeWeaponClass) {
				WeaponCreated = SpawnWeapon(DefaultMeleeWeaponClass);
			}
		}
	}
	break;
	case EWeaponType::EWT_Melee:
		if (DefaultMeleeWeaponClass) {
			WeaponCreated = SpawnWeapon(DefaultMeleeWeaponClass);
		}
		break;
	case EWeaponType::EWT_Fire:
		if (DefaultFireWeaponClass) {
			WeaponCreated = SpawnWeapon(DefaultFireWeaponClass);
		}
		break;
	case EWeaponType::EWT_Throwable:
		if (DefaultThrowableWeaponClass) {
			WeaponCreated = SpawnWeapon(DefaultThrowableWeaponClass);
		}
		break;
	case EWeaponType::EWT_Force:
		if (DefaultCyberWeaponClass) {
			WeaponCreated = SpawnWeapon(DefaultCyberWeaponClass);
		}
		break;
	}

	return WeaponCreated;
}

template <typename T>
T* AMainCharacter::SpawnWeapon(TSubclassOf<T> WeaponClass) {
	return GetWorld()->SpawnActor<T>(WeaponClass);
}

void AMainCharacter::EquipWeapon(AWeapon* WeaponToEquip, FName SocketName, bool bSwapping) {
	if (WeaponToEquip) {
		// WeaponToEquip->GetAreaSphere()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		// WeaponToEquip->GetCollisionBox()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(SocketName);
		if (Socket) {
			Socket->AttachActor(WeaponToEquip, GetMesh());
		}

		//TODO: create delegate
		//TODO: Create animation for picking up weapon.
		if (EquippedWeapon == nullptr) {

		}
		else if (!bSwapping) {

		}

		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void AMainCharacter::DropItem(AItem* ItemToDrop) {
	if (ItemToDrop) {
		ItemToDrop->GetItemMesh()->DetachFromComponent({EDetachmentRule::KeepWorld, true});
		ItemToDrop->SetItemState(EItemState::EIS_Falling);
		ItemToDrop->ThrowItem();

		ItemToDrop = nullptr;
	}
}

void AMainCharacter::SwapWeapon(AWeapon* WeaponToSwap) {
	DropItem(EquippedWeapon);
	SetEquippedWeapon(nullptr);
	EquipWeapon(WeaponToSwap);
}

void AMainCharacter::InteractButtonPressed() {
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (TraceHitItem) {
		TraceHitItem->InteractWithItem(this);
		TraceHitItem = nullptr;
	}
}

void AMainCharacter::InventoryButtonPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Inventory Button"));
}

void AMainCharacter::PauseButtonPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Pause Button"));
}

void AMainCharacter::DebugDropItem() {
	DropItem(EquippedWeapon);
	SetEquippedWeapon(nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Item Dropped"));
}

void AMainCharacter::QuickSelectButtonPressed(float Value) {
	if (Value > 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Quick Select Button"));
	}
}

void AMainCharacter::SphereOverlapBegin(FGuid Guid) {
	OverlappedItemIDs.Add(Guid);
	bShouldTraceForItems = true;
	UE_LOG(LogTemp, Warning, TEXT("SphereOverlapBegin %s"), *Guid.ToString());
}

void AMainCharacter::SphereOverlapEnd(FGuid Guid) {
	OverlappedItemIDs.Remove(Guid);
	bShouldTraceForItems = false;
	UE_LOG(LogTemp, Warning, TEXT("SphereOverlapEnd %s"), *Guid.ToString());
}

void AMainCharacter::CoverSystem() {
	FVector OutStart;
	FVector OutEnd;
	bool bCovering = GetForwardTracers(OutStart, OutEnd);
	if (bInCover && bCoverActive && bCoveringActive) {
		bCanCover = bCovering;
		if (!bCanCover) {
			if (PoseType != EPoseType::EPT_Climb) {
				bCoverActive = false;
				bCoveringActive = false;
				bInCover = false;
				SwitchCamera(false);
			}
		}
	}

	if (bInCover) {
		LeftTracer();
		RightTracer();
	}

	// UE_LOG(LogTemp, Warning, TEXT("CanCover: %s CoverActive: %s InCover: %s"), bCanCover ? TEXT("true"): TEXT("false"),
	//        bCoverActive ? TEXT("true"): TEXT("false"), bInCover ? TEXT("true"): TEXT("false"))
}

void AMainCharacter::EnterCover() {
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->CanCover_Implementation(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bInCover = true;
	SwitchCamera(true);

	FRotator CoverRot = UKismetMathLibrary::MakeRotFromX(CoverNormal);
	FRotator TargetRot = FRotator(CoverRot.Pitch, CoverRot.Yaw
	                              - 180.0f
	                              , CoverRot.Roll);

	FLatentActionInfo Info = FLatentActionInfo();
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(),
	                                      MoveToLocation(),
	                                      TargetRot,
	                                      false,
	                                      false,
	                                      0.2f,
	                                      false,
	                                      EMoveComponentAction::Type::Move,
	                                      Info);
}

void AMainCharacter::ExitCover() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->CanCover_Implementation(false);
	bInCover = false;
	SwitchCamera(false);
}

void AMainCharacter::LeftTracer() {
	bMoveLeft = CoverTracer(CoverLeftMovement);
}

void AMainCharacter::RightTracer() {
	bMoveRight = CoverTracer(CoverRightMovement);
}

bool AMainCharacter::CoverTracer(UArrowComponent* AComponent) {
	TArray<AActor*> IgnoredActors;
	FHitResult OutHitResult;
	return UKismetSystemLibrary::CapsuleTraceSingle(this,
	                                                AComponent->GetComponentLocation(),
	                                                AComponent->GetComponentLocation(),
	                                                20.0f,
	                                                60.0f,
	                                                ETraceTypeQuery::TraceTypeQuery1,
	                                                false,
	                                                IgnoredActors,
	                                                EDrawDebugTrace::ForOneFrame,
	                                                OutHitResult,
	                                                true
	);
}

void AMainCharacter::InCoverMoving() {
	if (bInCover) {
		UE_LOG(LogTemp, Warning, TEXT("Move Right Value: %f"), MoveRightValue)
		MoveLeftRight_Implementation(MoveRightValue);
		MoveInCover();
	}
	else {
		bMoveRight = false;
		bMoveLeft = false;
	}
}

void AMainCharacter::MoveInCover() {
	FVector Loc = GetActorLocation();

	if (bMoveRight) {
		if (MoveRightValue > 0) {
			FVector Vec = UKismetMathLibrary::VInterpTo(Loc,
			                                            (GetActorRotation().Quaternion().GetRightVector() * 20.0f) +
			                                            Loc,
			                                            GetWorld()->GetDeltaSeconds(),
			                                            5.0f
			);
			SetActorLocation(Vec);
			bMoveRight = true;
			bMoveLeft = false;
		}
	}

	if (bMoveLeft) {
		if (MoveRightValue < 0) {
			FVector Vec = UKismetMathLibrary::VInterpTo(Loc,
			                                            (GetActorRotation().Quaternion().GetRightVector() * -20.0f) +
			                                            Loc,
			                                            GetWorld()->GetDeltaSeconds(),
			                                            5.0f
			);
			SetActorLocation(Vec);
			bMoveRight = true;
			bMoveLeft = false;
		}
	}

	if (MoveRightValue == 0.0f) {
		bMoveRight = false;
		bMoveLeft = false;
	}
}

void AMainCharacter::CoverMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	UE_LOG(LogTemp, Warning, TEXT("Montage End Name: %s"), *Montage->GetName());
	bCoverMontageEnded = true;
}

FTransform AMainCharacter::SetCameraTransform(UCameraComponent* Camera, FName SocketName, bool AttackComponent,
                                              USkeletalMeshComponent* Parent) const {
	Camera->DetachFromComponent(
		FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			true)
	);
	FTransform CameraTransform = Camera->GetRelativeTransform();
	if (AttackComponent) {
		Camera->AttachToComponent(Parent,
		                          FAttachmentTransformRules(
			                          EAttachmentRule::KeepWorld,
			                          EAttachmentRule::KeepWorld,
			                          EAttachmentRule::KeepWorld,
			                          true
		                          ),
		                          SocketName);
	}

	return CameraTransform;
}

void AMainCharacter::SetActiveCameras(bool FollowCameraActive) {
	if (FollowCameraActive) {
		CameraState = ECameraState::ECS_EyesCamera;
	}
	else {
		CameraState = ECameraState::ECS_FollowCamera;
	}
	FollowCamera->SetActive(FollowCameraActive);
	EyesCamera->SetActive(!FollowCameraActive);
	RefFollowCamera->SetActive(false);
	EyesCameraHeadComponent->SetOwnerNoSee(!FollowCameraActive);
	EyesCameraHeadComponent->bCastHiddenShadow = !FollowCameraActive;
}

void AMainCharacter::OnCameraTimelineFloatUpdate(float Output) {
	FTransform CameraTrack = UKismetMathLibrary::TLerp(
		FollowCameraTransform,
		EyesCameraTransform,
		Output
	);
	FVector CameraLoc = CameraTrack.GetLocation();
	FQuat CameraRot = CameraTrack.GetRotation();
	FVector CameraScale = CameraTrack.GetScale3D();
	FollowCamera->SetRelativeTransform(FTransform(
			FRotator(CameraRot.Y, CameraRot.Z, CameraRot.X),
			CameraLoc,
			CameraScale
		)
	);

	const FRotator EyesCameraRotation = EyesCamera->GetComponentRotation();
	FVector EyesCameraLocation = EyesCamera->GetRelativeLocation();
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	UE_LOG(LogTemp, Warning,
	       TEXT("Eyes Rotation Pitch: %f Eyes Location Y: %f Transition Location Z: %f Transition Rotation Z: %f"),
	       EyesCameraRotation.Pitch, EyesCameraLocation.Y, CameraLoc.Z, CameraRot.Z, Output)
	Player->SetControlRotation(FRotator(EyesCameraRotation.Pitch, CameraRot.Z, 0.0f));
}

void AMainCharacter::OnCameraTimelineFinished() {
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	if (bSwitchToFollowCamera) {
		FollowCamera->AttachToComponent(CameraBoom,
		                                FAttachmentTransformRules(
			                                EAttachmentRule::SnapToTarget,
			                                EAttachmentRule::SnapToTarget,
			                                EAttachmentRule::SnapToTarget,
			                                true
		                                ));
		if (RefFollowCameraRotation) {
			Player->SetControlRotation(RefFollowCameraRotation->GetComponentRotation());
		}

		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = bRotationYaw;
		bUseControllerRotationPitch = false;
	}
	else {
		SetActiveCameras(false);

		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = true;
		bUseControllerRotationPitch = false;
		UE_LOG(LogTemp, Warning, TEXT("Is Eyes Camera Attached To Mesh: %s"),
		       EyesCamera->IsAttachedTo(GetMesh()) ? TEXT("true") : TEXT("false"));
		// EyesCamera->SetupAttachment(GetMesh(), "head");
	}
	EnableInput(Player);
}

void AMainCharacter::UpdateClimbingTransitionTimeline(float Output) {
	UE_LOG(LogTemp, Warning, TEXT("UpdateClimbingTransitionTimeline %f"), Output);
	if (Controller) {
		FRotator Rotation = Controller->GetControlRotation();
		if (ClimbRightValue != 0) {
			float CurrentYaw = Rotation.Yaw + (45.0f * ClimbRightValue);
			float LerpYaw;
			if (ClimbRightValue > 0.0f) {
				if (bClimbingButtonPressed)
					LerpYaw = FMath::Lerp(ClimbStartRotationYaw, ClimbStartRotationYawLeft, Output);
				else
					LerpYaw = FMath::Lerp(ClimbStartRotationYawLeft, ClimbStartRotationYaw, Output);
				// CurrentYaw = ClimbStartRotationYawLeft;
				CurrentYaw = LerpYaw;
			}
			else if (ClimbRightValue < 0.0f) {
				if (bClimbingButtonPressed)
					LerpYaw = FMath::Lerp(ClimbStartRotationYaw, ClimbStartRotationYawRight, Output);
				else
					LerpYaw = FMath::Lerp(ClimbStartRotationYawRight, ClimbStartRotationYaw, Output);
				// CurrentYaw = ClimbStartRotationYawRight;
				CurrentYaw = LerpYaw;
			}
			Rotation.Yaw = CurrentYaw;
			// AddControllerYawInput(CurrentYaw);
			// if (CurrentYaw <= ClimbStartRotationYawLeft && CurrentYaw >= ClimbStartRotationYawRight)
			// AddControllerYawInput(CurrentYaw);
		}
		else {
			Rotation.Yaw = ClimbStartRotationYaw;
		}

		Controller->SetControlRotation(Rotation);
		bPlayClimbTurnAnimation = false;
	}
}

void AMainCharacter::UpdateAimTransitionTimeline(float Output) {
	CurrentAimValue = Output;
	UE_LOG(LogTemp, Warning, TEXT("Output: %f"), Output)
}

FVector AMainCharacter::MoveToLocation() const {
	FVector MultipliedCoverNormal = CoverNormal * FVector(22.0f, 22.0f, 0.0f);
	float MultipliedCoverNormalX = MultipliedCoverNormal.X + CoverLocation.X;
	float MultipliedCoverNormalY = MultipliedCoverNormal.Y + CoverLocation.Y;
	return FVector(MultipliedCoverNormalX, MultipliedCoverNormalY, CoverLocation.Z);
}

void AMainCharacter::CanCover_Implementation(bool bCover) {

}

void AMainCharacter::MoveLeftRight_Implementation(float Direction) {
	UE_LOG(LogTemp, Warning, TEXT("Direction Char %f"), Direction)
}

float AMainCharacter::GetCrosshairSpreadMultiplier() const {
	return CrosshairSpreadMultiplier;
}
