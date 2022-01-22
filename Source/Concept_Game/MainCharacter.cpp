// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "ActionComponent.h"
#include "Cover.h"
// #include "CoverComponent.h"

#include "BaseHUDComponent.h"
#include "CoverComponent.h"
#include "DebugWidgetComponent.h"
#include "DialogComponent.h"
#include "DoorComponent.h"
#include "DoubleJumpSkill.h"
#include "DrawDebugHelpers.h"
#include "EnhancementComponent.h"
#include "FlashlightComponent.h"
#include "HackingComponent.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "Ladder.h"
#include "LockpickComponent.h"
#include "MainAnimInstance.h"
#include "MainCharacterCameraComponent.h"
#include "MainCharacterInputComponent.h"
#include "MapComponent.h"
#include "MeleeComponent.h"
#include "ParkourComponent.h"
#include "PauseMenuComponent.h"
#include "QuestComponent.h"
#include "RenderComponent.h"
#include "ShootingComponent.h"
#include "StealthComponent.h"
#include "RenderingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
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
	bRotationYaw(false),
	bFPSCounter(false),
	bCoverDisable(false),
	bCoverLeftDisable(false),
	bCoverRightDisable(false),
	bCoverActive(false),
	bMoveInCover(false),
	bCameraMoved(false),
	CoverToCoverAnimPlayRate(1.f),
	Tolerance(120.0f),
	bEnableCameraTransition(false),
	bSwitchToFollowCamera(false),
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
	ZoomInterpolationSpeed(20.0f),
	bUseWeaponButtonPressed(false),
	bAimingButtonPressed(false),
	bShouldAttack(false),
	bShouldTraceForItems(false),
	bJumpFromClimb(false),
	PoseAxisValueCounter(0),
	CameraDefaultFOV(0.0f),
	CameraZoomedFOV(35.0f),
	CameraCurrentFOV(0.0f),
	PoseType(EPoseType::EPT_Stand),
	bOverlappingLadderBottom(false),
	bOverlappingLadderTop(false),
	TimesJumped(0),
	LastPoseType(EPoseType::EPT_Stand),
	bRunning(false),
	CombatState(ECombatState::ECS_Unoccupied),
	CrouchMovementSpeed(300.0f),
	RunningMovementSpeed(650.0f),
	CrawlingMovementSpeed(250.0f),
	AimingMovementSpeed(350.0f),
	BaseMovementSpeed(350.0f),
	CrouchGroundFriction(100.0f),
	CrawlingGroundFriction(50.0f),
	BaseGroundFriction(2.0f),
	CrouchCharacterVisibility(50.0f),
	RunningCharacterVisibility(100.0f),
	CrawlingCharacterVisibility(30.0f),
	BaseCharacterVisibility(80.0f),
	StandingCapsuleHalfHeight(88.0f),
	CrouchingCapsuleHalfHeight(44.0f),
	CrawlingCapsuleHalfHeight(22.0f),
	Health(100.0f),
	MaxHealth(100.0f),
	CrosshairSpreadMultiplier(0.0f),
	CrosshairVelocityFactor(0.0f),
	CrosshairInAirFactor(0.0f),
	CrosshairShootingFactor(0.0f),
	bPlayClimbTurnAnimation(false) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("CharacterItemComponent"));
	CharacterRenderComponent = CreateDefaultSubobject<URenderComponent>(
		TEXT("CharacterPostProcessComponent"));

	ConstructCameraBoom();
	ConstructFollowCamera();
	ConstructEyesCamera();
	ConstructRefFollowCamera();
	ConstructRefFollowCameraArrowComponent();
	ConstructCoverArrows();

	CharacterCoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CharacterCoverComponent"));
	CharacterShootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("CharacterShootingComponent"));
	CharacterMeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("CharacterMeleeComponent"));
	CharacterParkourComponent = CreateDefaultSubobject<UParkourComponent>(TEXT("CharacterParkourComponent"));

	CharacterEnhancementComponent = CreateDefaultSubobject<
		UEnhancementComponent>(TEXT("CharacterEnhancementComponent"));
	CharacterPauseMenuComponent = CreateDefaultSubobject<UPauseMenuComponent>(TEXT("CharacterPauseMenuComponent"));
	CharacterInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("CharacterInventoryComponent"));
	CharacterSkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("CharacterSkillComponent"));

	CharacterCameraComponent = CreateDefaultSubobject<UMainCharacterCameraComponent>(TEXT("CharacterCameraComponent"));
	CharacterInputComponent = CreateDefaultSubobject<UMainCharacterInputComponent>(TEXT("CharacterInputComponent"));
	CharacterBaseHUDComponent = CreateDefaultSubobject<UBaseHUDComponent>(TEXT("CharacterBaseHUDComponent"));
	CharacterQuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("CharacterQuestComponent"));
	DebugWidgetComponent = CreateDefaultSubobject<UDebugWidgetComponent>(TEXT("DebugWidgetComponent"));
	CharacterDialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("CharacterDialogComponent"));
	CharacterStealthComponent = CreateDefaultSubobject<UStealthComponent>(TEXT("CharacterStealthComponent"));
	CharacterLockpickComponent = CreateDefaultSubobject<ULockpickComponent>(TEXT("CharacterLockpickComponent"));
	CharacterHackingComponent = CreateDefaultSubobject<UHackingComponent>(TEXT("CharacterHackingComponent"));
	CharacterDoorComponent = CreateDefaultSubobject<UDoorComponent>(TEXT("CharacterDoorComponent"));
	CharacterFlashLightComponent = CreateDefaultSubobject<UFlashlightComponent>(TEXT("CharacterFlashLightComponent"));
	CharacterMapComponent = CreateDefaultSubobject<UMapComponent>(TEXT("CharacterMapComponent"));

	FogComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("FogComponent"));
	FogComponent->SetupAttachment(RootComponent);

	AimTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTransitionTimeline"));

	ChangeCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ChangeCameraTimeline"));

	ClimbingTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ClimbingTransitionTimeline"));

	InvisibleCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("InvisibleCharacterMesh"));
	InvisibleCharacterMesh->SetupAttachment(RootComponent);
	InvisibleCharacterMesh->SetMasterPoseComponent(GetMesh());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	ConstructCharacterMovement();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	// InitSkills();

	SetDefaultCameras();

	SetTestWeapon();

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

void AMainCharacter::SetTestWeapon() {
	AWeapon* Weapon = SpawnDefaultWeapon();
	bool bAdded = GetCharacterInventoryComponent()->TryAddInventoryItem(Weapon);
	UE_LOG(LogTemp, Warning, TEXT("Added Test Weapon: %s => %s"), *GetName(),
	       bAdded ? TEXT("true") : TEXT("false"));
	if (bAdded) {
		// Destroy();
		// GetWorld()->DestroyActor(this);
		EquipWeapon(Weapon);
	}
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

// //ITEMS
// void AMainCharacter::TraceForItems() {
// 	// if (bShouldTraceForItems) {
// 	if (OverlappedItemIDs.Num() > 0) {
// 		FHitResult ItemTraceHitResult;
// 		FVector HitLocation;
// 		TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
// 		if (ItemTraceHitResult.bBlockingHit) {
// 			TraceHitItem = Cast<AItem>(ItemTraceHitResult.Actor);
//
// 			//TODO: If Trace hit item exists, switch between item types...
// 			if (TraceHitItem) {
// 				// UE_LOG(LogTemp, Error, TEXT("Tracing item: %s"), *TraceHitItem->GetName());
// 			}
//
// 			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterp) {
// 				TraceHitItem = nullptr;
// 			}
//
// 			if (TraceHitItem && TraceHitItem->GetPickupWidget()) {
// 				TraceHitItem->PerformTrace(this, OverlappedItemIDs);
// 			}
//
// 			if (TraceHitItemLastFrame) {
// 				if (TraceHitItem != TraceHitItemLastFrame) {
// 					TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
// 				}
// 			}
//
// 			TraceHitItemLastFrame = TraceHitItem;
// 		}
// 		else if (TraceHitItemLastFrame) {
// 			TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
// 		}
// 	}
// 	else if (TraceHitItemLastFrame) {
// 		TraceHitItemLastFrame->LeaveTrace(this, OverlappedItemIDs);
// 	}
//
// }

//TODO: Remove function when APhysicsInteractionItem will extend from AItem
// void AMainCharacter::TraceForPhysicsItems() {
//
// 	if (OverlappedItemIDs.Num() > 0) {
// 		FHitResult ItemTraceHitResult;
// 		FVector HitLocation;
// 		TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
// 	}
// }

//LADDER
// void AMainCharacter::TraceForLadder() {
// 	FHitResult LadderTraceHitResult;
// 	FVector HitLocation;
// 	bool Trace = TraceForLevelChange(LadderTraceHitResult, HitLocation);
// 	if (PoseType == EPoseType::EPT_Climb) {
// 		bJumpFromClimb = !Trace;
// 		bTouchingFloor = Trace;
// 	}
// }

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

	ConstructPostProcess();

	// TraceForItems();
	// TraceForPhysicsItems();
	// TraceForLadder();

	InterpCapsuleHalfHeight(DeltaTime);

	CoverSystem();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	CharacterInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainCharacter::ConstructCharacterMovement() const {
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void AMainCharacter::ConstructPostProcess() {
	if (CharacterRenderComponent) {
		FogComponent->bEnabled = CharacterRenderComponent->ApplyFog();
	}

	// UE_LOG(LogTemp, Warning, TEXT("FogComponent Enabled? %s"), FogComponent->bEnabled ? TEXT("TRUE"): TEXT("FALSE"))
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

bool AMainCharacter::TraceCoverMovement(float Orientation) {
	FHitResult OutHitResult;
	FVector CharacterNormalVector = GetCharacterMovement()->GetPlaneConstraintNormal() * Orientation;
	FRotator CoverRot = UKismetMathLibrary::MakeRotFromX(CharacterNormalVector);
	FVector StartTrace = GetActorLocation() + CoverRot.Quaternion().GetRightVector() * 80.f;
	FVector EndTrace = StartTrace + CharacterNormalVector * 200.f;

	TArray<AActor*> IgnoredActors;
	bool Trace = UKismetSystemLibrary::SphereTraceSingle(this,
	                                                     StartTrace,
	                                                     EndTrace,
	                                                     10.f,
	                                                     ETraceTypeQuery::TraceTypeQuery1,
	                                                     false,
	                                                     IgnoredActors,
	                                                     EDrawDebugTrace::Type::ForOneFrame,
	                                                     OutHitResult,
	                                                     true
	);

	return Trace;
}

void AMainCharacter::PeakLeft() {
	// UE_LOG(LogTemp, Warning, TEXT("Peak Overlap Left %s"), bCanPeakLeft ? TEXT("true") : TEXT("false"))
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->PeakLeft_Implementation(bCoverPeakLeft);
}

void AMainCharacter::PeakRight() {
	// UE_LOG(LogTemp, Warning, TEXT("Peak Overlap Right %s"), bCanPeakRight ? TEXT("true") : TEXT("false"))
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->PeakRight_Implementation(bCoverPeakRight);
}

void AMainCharacter::PeakTop() {
	UE_LOG(LogTemp, Warning, TEXT("Peak Overlap Top %s bCoverPeakTop: %s, ButtonPressed: %s"),
	       bCanPeakTop ? TEXT("true") : TEXT("false"),
	       bCoverPeakTop ? TEXT("true") : TEXT("false"),
	       bAimingButtonPressed ? TEXT("true") : TEXT("false"))
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->PeakTop_Implementation(bCoverPeakTop);
}

bool AMainCharacter::GetForwardTracers(FVector& OutStart, FVector& OutEnd, FHitResult& TraceHitResult) {
	FVector RotFVector = GetActorRotation().Quaternion().GetForwardVector();
	FVector MultipliedFVector = {RotFVector.X * 80.0f, RotFVector.Y * 80.0f, RotFVector.Z};
	float CoverRadius = 20.0f;
	OutStart = GetActorLocation();
	OutEnd = OutStart + MultipliedFVector;

	TArray<AActor*> IgnoredActors;
	// FHitResult OutHitResult;
	return UKismetSystemLibrary::SphereTraceSingle(this,
	                                               OutStart,
	                                               OutEnd,
	                                               CoverRadius,
	                                               ETraceTypeQuery::TraceTypeQuery1,
	                                               false,
	                                               IgnoredActors,
	                                               EDrawDebugTrace::Type::ForOneFrame,
	                                               TraceHitResult,
	                                               true
	);
}

bool AMainCharacter::GetInCoverMouseTracer(FVector& OutStart, FVector& OutEnd) {
	OutStart = GetActorLocation();

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
		OutEnd = OutStart + CrosshairWorldDirection * 200.0f;
		FHitResult OutHitResult;
		bool Trace = GetWorld()->LineTraceSingleByChannel(OutHitResult, OutStart, OutEnd, ECC_Visibility);
		// UE_LOG(LogTemp, Warning, TEXT("OutEnd Y: %f OutStart Y: %f"), OutEnd.Y, OutStart.Y)
		float PositionsDelta = OutStart.Y - OutEnd.Y;
		if (!bStoreTolerance) {
			Tolerance = OutEnd.Y;
			bStoreTolerance = true;
		}
		FVector LM = CoverLeftMovement->GetComponentLocation();
		FVector RM = CoverRightMovement->GetComponentLocation();
		FVector TM = CoverTopMovement->GetComponentLocation();


		//TODO: Find location of Cover and Make movement for it.
		float CurrentLM;
		float CurrentRM;
		float CurrentTM;
		float CurrentOutEnd;
		bUseXInCoverMovement = LM.Y == RM.Y;
		if (bUseXInCoverMovement) {
			CurrentLM = LM.X;
			CurrentRM = RM.X;
			CurrentTM = TM.X;
			CurrentOutEnd = OutEnd.X;
		}
		else {
			CurrentLM = LM.Y;
			CurrentRM = RM.Y;
			CurrentTM = TM.Y;
			CurrentOutEnd = OutEnd.Y;
		}

		TArray<float> MovementOptions = {CurrentLM, CurrentRM, CurrentTM};
		float MaxOption = FMath::Max(MovementOptions);

		float FinalOption = 0;
		for (float MO : MovementOptions) {
			if (FMath::Abs(MO - CurrentOutEnd) < MaxOption) {
				MaxOption = FMath::Abs(MO - CurrentOutEnd);
				FinalOption = MO;
			}
		}

		UE_LOG(
			LogTemp, Warning,
			TEXT(
				"OutStart: %s OutEnd: %f LM: %f RM: %f TM: %f, Option: %f, MaxOption: %f"
			),
			*OutStart.ToString(),
			CurrentOutEnd,
			CurrentLM,
			CurrentRM,
			CurrentTM,
			FinalOption,
			MaxOption
		)

		if (FinalOption == CurrentLM) {
			bMoveForwardDisable = false;
			bMouseRightForwardMove = true;
			bMouseLeftForwardMove = false;
			UE_LOG(LogTemp, Warning, TEXT("Brute Right "))
		}
		else if (FinalOption == CurrentRM) {
			bMoveForwardDisable = false;
			bMouseRightForwardMove = false;
			bMouseLeftForwardMove = true;
			UE_LOG(LogTemp, Warning, TEXT("Brute Left "))
		}
		else if (FinalOption == CurrentTM) {
			bMouseRightForwardMove = false;
			bMouseLeftForwardMove = false;
			bMoveForwardDisable = true;
			UE_LOG(LogTemp, Warning, TEXT("Brute Enable Movement"))
		}
		return Trace;

	}

	return false;
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

	if (CharacterItemComponent) {
		CharacterItemComponent->GrabHandleComponent->SetupAttachment(EyesCamera);
	}
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
	CoverTopMovement = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverTopMovement"));
	CoverTopMovement->SetupAttachment(GetCapsuleComponent());

	CoverDisableLeftMovement = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverDisableLeftMovement"));
	CoverDisableLeftMovement->SetupAttachment(GetCapsuleComponent());
	CoverDisableRightMovement = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverDisableRightMovement"));
	CoverDisableRightMovement->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::ConstructEyesCameraHeadComponent() {
	EyesCameraHeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyesCameraHeadComponent"));
	EyesCameraHeadComponent->SetupAttachment(GetMesh(), "head");
	EyesCameraHeadComponent->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
	EyesCameraHeadComponent->SetOwnerNoSee(true);
	EyesCameraHeadComponent->bCastHiddenShadow = true;

	InvisibleCameraHeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("InvisibleCameraHeadComponent"));
	InvisibleCameraHeadComponent->SetupAttachment(GetMesh(), "head");
	InvisibleCameraHeadComponent->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
	InvisibleCameraHeadComponent->SetOwnerNoSee(true);
	InvisibleCameraHeadComponent->bCastHiddenShadow = true;
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

	UseWeaponByType(EquippedWeapon->GetWeaponType());

}

void AMainCharacter::Jump() {
	if (PoseType != EPoseType::EPT_Climb) {
		if (PoseType != EPoseType::EPT_Stand && (!bInCover || !bCoverActive || !bCoveringActive)) {
			UE_LOG(LogTemp, Warning, TEXT("Jumping Not Stand"));
			PoseType = EPoseType::EPT_Stand;
			GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		}
		else if (bInCover || bCoverActive || bCoveringActive) {
			if (!bSlideToLeftCover && !bSlideToRightCover) {
				//TODO: Remove duplicates, to make code clearer
				if (CurrentCoverPoint->GetOverlappingCover() != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("Change Cover"))
					bMoveInCover = true;

					if (CurrentCover)
						CurrentCover->GetRootComponent()->SetVisibility(true);

					FRotator CoverRot = UKismetMathLibrary::MakeRotFromX(CurrentCoverPoint->GetCoverNormal());
					FRotator TargetRot = FRotator(CoverRot.Pitch, CoverRot.Yaw
					                              - 180.0f
					                              , CoverRot.Roll);
					FLatentActionInfo Info = FLatentActionInfo();
					Info.CallbackTarget = this;
					Info.ExecutionFunction = "MoveBetweenCovers";
					Info.Linkage = 0;
					Info.UUID = FMath::Rand();

					float Dist = FVector::Dist(GetActorLocation(), CurrentCoverPoint->GetActorLocation());

					GetCapsuleComponent()->SetWorldRotation(TargetRot);

					FVector NewLocation = {
						CurrentCoverPoint->GetActorLocation().X, CurrentCoverPoint->GetActorLocation().Y,
						GetActorLocation().Z
					};

					UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(),
					                                      NewLocation,
					                                      TargetRot,
					                                      false,
					                                      false,
					                                      Dist / GetCharacterMovement()->MaxWalkSpeed,
					                                      false,
					                                      EMoveComponentAction::Type::Move,
					                                      Info);
				}
			}
			else {
				// Move Camera back to previous location
				if (bCameraMoved) {
					FollowCamera->SetRelativeLocation(RefFollowCameraLocation);
				}
				RefFollowCameraLocation = FollowCamera->GetRelativeLocation();
				bCameraMoved = false;

				if (CurrentCover)
					CurrentCover->GetRootComponent()->SetVisibility(true);


				FRotator CoverRot = UKismetMathLibrary::MakeRotFromX(CoverPointEndTrace->GetCoverNormal());
				// FRotator TargetRot = FRotator(
				// 	CoverRot.Pitch,
				// 	CoverRot.Yaw
				// 	// - 180.0f
				// 	, CoverRot.Roll
				// );

				FRotator TargetRot = GetActorRotation();

				FLatentActionInfo Info = FLatentActionInfo();
				Info.CallbackTarget = this;
				Info.ExecutionFunction = "MoveBetweenCovers";
				Info.Linkage = 0;
				Info.UUID = FMath::Rand();

				float Dist = FVector::Dist(GetActorLocation(), CoverPointEndTrace->GetActorLocation());

				GetCapsuleComponent()->SetWorldRotation(TargetRot);

				FVector NewLocation = {
					CoverPointEndTrace->GetActorLocation().X,
					CoverPointEndTrace->GetActorLocation().Y,
					GetActorLocation().Z
				};


				//TODO: Calculate distances between covers
				if (bSlideToLeftCover) {
					UE_LOG(LogTemp, Warning, TEXT("Can Slide To Left"))
				}

				if (bSlideToRightCover) {
					UE_LOG(LogTemp, Warning, TEXT("Can Slide To Right"))
				}

				UE_LOG(LogTemp, Warning,
				       TEXT("Move To: CoverLocation: %s CoverRot %s TargetRot %s Dist %f NewLocation %s"),
				       *CoverPointEndTrace->GetActorLocation().ToString(),
				       *CoverRot.ToString(),
				       *TargetRot.ToString(),
				       Dist,
				       *NewLocation.ToString())

				CoverToCoverAnimPlayRate = Dist / GetCharacterMovement()->MaxWalkSpeed;

				UE_LOG(LogTemp, Warning, TEXT("CoverToCoverAnimPlayRate: %f"), CoverToCoverAnimPlayRate)

				bMoveInCover = true;

				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(),
				                                      NewLocation,
				                                      TargetRot,
				                                      false,
				                                      false,
				                                      CoverToCoverAnimPlayRate,
				                                      false,
				                                      EMoveComponentAction::Type::Move,
				                                      Info);
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Jumping Base"));
			Super::Jump();

			if (TimesJumped < 2) {
				if (GetCharacterMovement()->IsFalling()) {
					//=== UNCOMMENT TO DOUBLE JUMP
					// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					//===

					// LaunchCharacter(FVector(0.0f, 0.0f, 500.f), false, true);
				}
				//=== UNCOMMENT TO DOUBLE JUMP
				// Super::Jump();
				// TimesJumped++;
				//===
			}
		}
	}
	else {
		bJumpFromClimb = true;
		PoseType = EPoseType::EPT_Stand;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		SwitchCamera(false);
		UE_LOG(LogTemp, Warning, TEXT("Jumping From Climb %s"), bJumpFromClimb ? TEXT("true") : TEXT("false"));
		Super::Jump();
		if (TimesJumped < 2) {
			if (GetCharacterMovement()->IsFalling()) {
				//=== UNCOMMENT TO DOUBLE JUMP
				// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				//===

				// LaunchCharacter(FVector(0.0f, 0.0f, 500.f), false, true);
			}
			//=== UNCOMMENT TO DOUBLE JUMP
			// Super::Jump();
			// TimesJumped++;
			//===
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Times Jumped: %i"), TimesJumped)

}

void AMainCharacter::StopJumping() {
	Super::StopJumping();
}

void AMainCharacter::Landed(const FHitResult& MovieSceneBlends) {
	TimesJumped = 0;
	Super::Landed(MovieSceneBlends);
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

template <typename T>
T* AMainCharacter::SpawnCoverPoint(TSubclassOf<T> CoverPortalClass
	// , FVector& CoverLocation, FRotator& CoverRotation
) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	return GetWorld()->SpawnActor<T>(CoverPortalClass
		//, CoverLocation, CoverRotation, SpawnParams
	);
}

bool AMainCharacter::RemoveCoverPoint(ACoverPoint* Point) {
	return GetWorld()->DestroyActor(Point);
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
	FHitResult TracerResult;
	bool bCovering = GetForwardTracers(OutStart, OutEnd, TracerResult);
	TraceCoverDisable();

	if (!bMoveInCover) {
		if (bInCover && bCoverActive && bCoveringActive) {
			if (bCoverMontageEnded) {
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
		}

		if (bInCover) {
			CurrentCoverHitResult = TracerResult;
			CurrentCover = Cast<ACover>(TracerResult.Actor);
			if (CurrentCover) {
				CurrentCover->SetCurrentOverlappingCharacter(this);
				CurrentCover->InCoverSystem();
			}
		}
		else {
			bStoreTolerance = false;
		}
	}
}

void AMainCharacter::EnterCover() {
	TopTracer();
	if (bCanPeakTop && PoseType != EPoseType::EPT_Crouch)
		PoseType = EPoseType::EPT_Crouch;

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

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	// GetCharacterMovement()->SetPlaneConstraintNormal(CoverNormal);
	bUseControllerRotationYaw = false;

	PlayMontage(ECharacterMontage::ECM_TakeCover,
	            EquippedWeapon != nullptr
		            ? EquippedWeapon->GetWeaponType()
		            : EWeaponType::EWT_Any);
}

void AMainCharacter::ExitCover() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance())->CanCover_Implementation(false);
	bInCover = false;
	SwitchCamera(false);
	GetCharacterMovement()->SetPlaneConstraintEnabled(false);
	if (CurrentCover)
		CurrentCover->GetRootComponent()->SetVisibility(true);
}


void AMainCharacter::LeftTracer() {
	FHitResult OutHitResult;
	float CapsuleHeight = PoseType == EPoseType::EPT_Crouch ? 20.0f : 60.0f;

	bMoveLeft = CoverTracer(CoverLeftMovement, OutHitResult, CapsuleHeight);

	CoverLeftSphereBlocker = OutHitResult.GetActor();
	if (OutHitResult.bBlockingHit) {
		bCanPeakLeft = false;
		if (!bCanPeakRight) {
			if (bCameraMoved) {
				FollowCamera->SetRelativeLocation(RefFollowCameraLocation);
			}
			RefFollowCameraLocation = FollowCamera->GetRelativeLocation();
			bCameraMoved = false;
		}
	}
	else {
		if (!bCameraMoved) {
			FollowCamera->SetRelativeLocation(
				FVector(RefFollowCameraLocation.X,
				        RefFollowCameraLocation.Y - 120.0f,
				        RefFollowCameraLocation.Z)
			);
			bCameraMoved = true;
		}
		bCanPeakLeft = true;
	}
}

void AMainCharacter::RightTracer() {
	FHitResult OutHitResult;
	float CapsuleHeight = PoseType == EPoseType::EPT_Crouch ? 20.0f : 60.0f;

	bMoveRight = CoverTracer(CoverRightMovement, OutHitResult, CapsuleHeight);

	CoverRightSphereBlocker = OutHitResult.GetActor();
	if (OutHitResult.bBlockingHit) {
		bCanPeakRight = false;
		if (!bCanPeakLeft) {
			if (bCameraMoved) {
				FollowCamera->SetRelativeLocation(RefFollowCameraLocation);
			}
			RefFollowCameraLocation = FollowCamera->GetRelativeLocation();
			bCameraMoved = false;
		}
	}
	else {
		if (!bCameraMoved) {
			FollowCamera->SetRelativeLocation(
				FVector(RefFollowCameraLocation.X,
				        RefFollowCameraLocation.Y + 80.0f,
				        RefFollowCameraLocation.Z)
			);
			bCameraMoved = true;
		}

		bCanPeakRight = true;
	}
}

void AMainCharacter::TopTracer() {
	FHitResult OutHitResult;
	bMoveTop = CoverTracer(CoverTopMovement, OutHitResult, 30.0f);
	if (OutHitResult.bBlockingHit) {
		bCanPeakTop = false;
	}
	else {
		bCanPeakTop = true;
	}

	if (bCanPeakTop && PoseType != EPoseType::EPT_Crouch)
		PoseType = EPoseType::EPT_Crouch;

	// CameraBoom->bDoCollisionTest = !bCanPeakTop;

	if (bCoverPeakTop) {
		if (!bAimingButtonPressed) {
			if (bCameraMoved) {
				FollowCamera->SetRelativeLocation(RefFollowCameraLocation);
			}
			RefFollowCameraLocation = FollowCamera->GetRelativeLocation();
			bCameraMoved = false;
		}
		else {
			if (!bCameraMoved) {
				FollowCamera->SetRelativeLocation(
					FVector(RefFollowCameraLocation.X,
					        RefFollowCameraLocation.Y,
					        RefFollowCameraLocation.Z + 80.f)
				);
				bCameraMoved = true;
			}
		}

	}
}

void AMainCharacter::HideCoverOnCameraTrace() {
	if (CurrentCoverHitResult.GetActor() != nullptr) {

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
			const FVector End = Start + CrosshairWorldDirection * 1000.f;
			FHitResult HitLocation;
			TArray<AActor*> IgnoredActors = {CurrentCoverHitResult.GetActor()};
			TArray<AActor*> IA;

			UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery2, false,
			                                      IgnoredActors, EDrawDebugTrace::ForOneFrame, HitLocation, false);

			FHitResult NextCoverHitResult;
			FVector NextCoverTraceStart = GetActorLocation();
			if (bCanPeakLeft) {
				NextCoverTraceStart = CoverLeftMovement->GetComponentLocation();
			}
			else if (bCanPeakRight) {
				NextCoverTraceStart = CoverRightMovement->GetComponentLocation();
			}
			FRotator NextCoverRotation = GetActorRotation();
			FVector NextCoverTraceEnd = NextCoverTraceStart +
				CrosshairWorldDirection
				* 1000.f;
			UKismetSystemLibrary::LineTraceSingle(this,
			                                      NextCoverTraceStart,
			                                      NextCoverTraceEnd,
			                                      ETraceTypeQuery::TraceTypeQuery1,
			                                      false,
			                                      IgnoredActors,
			                                      EDrawDebugTrace::ForOneFrame,
			                                      NextCoverHitResult,
			                                      false);

			if (NextCoverHitResult.bBlockingHit) {
				if (NextCoverHitResult.GetActor() != CurrentCoverHitResult.GetActor()) {
					if (CurrentCoverPoint == nullptr) {
						UE_LOG(LogTemp, Warning, TEXT("SPAWN NEW PORTAL"));
						CurrentCoverPoint = SpawnCoverPoint(DefaultCoverPointClass);
						CurrentCoverPoint->AttachCharacter(this);
					}

					if (CurrentCoverPoint != nullptr) {
						CurrentCoverPoint->SetActorLocation(NextCoverHitResult.Location);
						CurrentCoverPoint->SetCoverNormal(NextCoverHitResult.Normal);
					}
				}
				else {
					if (CurrentCoverPoint != nullptr) {
						if (RemoveCoverPoint(CurrentCoverPoint)) {
							CurrentCoverPoint = nullptr;
						}
					}
				}
			}
			else {
				if (CurrentCoverPoint != nullptr) {
					if (RemoveCoverPoint(CurrentCoverPoint)) {
						CurrentCoverPoint = nullptr;
					}
				}

			}

			if (HitLocation.bBlockingHit) {
				FHitResult HitPlayer;

				bool bPlayerIsOverlapped = UKismetSystemLibrary::LineTraceSingle(this,
					Start,
					GetActorLocation(),
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					IA,
					EDrawDebugTrace::None,
					HitPlayer,
					false
				);

				if (bPlayerIsOverlapped) {
					//TODO: Make Shadow visible only mesh invisible
					if (HitPlayer.GetActor() == CurrentCoverHitResult.GetActor()) {
						CurrentCoverHitResult.GetActor()->GetRootComponent()->SetVisibility(false);
					}
					else {
						CurrentCoverHitResult.GetActor()->GetRootComponent()->SetVisibility(true);
					}
				}
				else {
					CurrentCoverHitResult.GetActor()->GetRootComponent()->SetVisibility(true);
				}
			}
		}
	}
}

void AMainCharacter::MoveRightInCover(float Value) {
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = {0, Rotation.Yaw, 0};
	if (bMoveLeft && bMoveRight && !bCoverRightDisable && !bCoverLeftDisable) {
		FHitResult Res;

		TArray<AActor*> IgnoredActors;
		bool bTrace = UKismetSystemLibrary::SphereTraceSingle(this,
		                                                      GetActorLocation(),
		                                                      GetActorLocation() + GetCharacterMovement()->
		                                                      GetPlaneConstraintNormal() * -1.0f
		                                                      * 300.0f,
		                                                      60.f,
		                                                      ETraceTypeQuery::TraceTypeQuery1,
		                                                      false,
		                                                      IgnoredActors,
		                                                      EDrawDebugTrace::Type::ForOneFrame,
		                                                      Res,
		                                                      true);
		if (bTrace) {
			GetCharacterMovement()->SetPlaneConstraintNormal(Res.Normal);
			bTraceCoverLeft = Value > 0;
			bTraceCoverRight = Value < 0;

			AddMovementInput(YawRotation.Quaternion().GetRightVector(), Value);
		}
	}
	else {
		float CurrentValue = 0.0f;
		const float ValueSign = FMath::Sign(Value);
		bool bDir;
		if (ValueSign == 1.f) {
			bDir = bMoveRight && !bCoverRightDisable;
		}
		else {
			bDir = bMoveLeft && !bCoverLeftDisable;
		}

		if (ValueSign != 0.0f && bDir) {
			CurrentValue = Value;
		}

		bTraceCoverLeft = CurrentValue > 0;
		bTraceCoverRight = CurrentValue < 0;

		if (!bTraceCoverLeft && CurrentValue == 0.0f) {
			bTraceCoverLeft = bCanPeakLeft;
		}

		if (!bTraceCoverRight && CurrentValue == 0.0f) {
			bTraceCoverRight = bCanPeakRight;
		}

		AddMovementInput(YawRotation.Quaternion().GetRightVector(), CurrentValue);
	}
}

void AMainCharacter::TraceCharacterCover() {
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	const FVector Start = GetActorLocation();
	const FVector End = GetActorUpVector() * (-100.0f) + GetActorLocation();
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery1, false,
	                                      IgnoredActors, EDrawDebugTrace::ForOneFrame, HitResult, false);
}

void AMainCharacter::TraceCoverDisable() {
	if (!bInCover) {
		bCoverDisable = FrontTraceCoverDisable();
	}
}

void AMainCharacter::TraceCoverDisableWhileInCover() {
	if (bInCover) {
		bCoverLeftDisable = LeftTraceCoverDisable();
		bCoverRightDisable = RightTraceCoverDisable();
	}
}

bool AMainCharacter::FrontTraceCoverDisable() {
	FVector RotFVector = GetActorRotation().Quaternion().GetForwardVector();
	FVector MultipliedFVector = {RotFVector.X * 80.0f, RotFVector.Y * 80.0f, RotFVector.Z};
	FVector OutStart = {GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 20.f};
	FVector OutEnd = OutStart + MultipliedFVector;
	TArray<AActor*> IgnoredActors;
	FHitResult HitResult;
	ETraceTypeQuery CoverDisableTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3);
	bool Hit = UKismetSystemLibrary::SphereTraceSingle(this,
	                                                   OutStart,
	                                                   OutEnd,
	                                                   10.f,
	                                                   CoverDisableTraceType,
	                                                   false,
	                                                   IgnoredActors,
	                                                   EDrawDebugTrace::ForOneFrame,
	                                                   HitResult,
	                                                   true,
	                                                   FLinearColor::Yellow,
	                                                   FLinearColor::Blue);

	if (HitResult.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Front TraceCover Disabled Actor Name %s"), *HitResult.GetActor()->GetName())
	}
	return Hit;
}

bool AMainCharacter::LeftTraceCoverDisable() {
	ETraceTypeQuery CoverDisableTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3);
	FHitResult OutHitResult;
	float CapsuleHeight = PoseType == EPoseType::EPT_Crouch ? 20.0f : 60.0f;
	bool Hit = CoverTracer(CoverDisableLeftMovement,
	                       OutHitResult,
	                       CapsuleHeight,
	                       CoverDisableTraceType,
	                       FLinearColor::Yellow,
	                       FLinearColor::Blue);

	if (OutHitResult.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Left TraceCover Disabled Actor Name %s"),
		       *OutHitResult.GetActor()->GetName())
	}
	return Hit;
}

bool AMainCharacter::RightTraceCoverDisable() {
	ETraceTypeQuery CoverDisableTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3);
	FHitResult OutHitResult;
	float CapsuleHeight = PoseType == EPoseType::EPT_Crouch ? 20.0f : 60.0f;
	bool Hit = CoverTracer(CoverDisableRightMovement,
	                       OutHitResult,
	                       CapsuleHeight,
	                       CoverDisableTraceType,
	                       FLinearColor::Yellow,
	                       FLinearColor::Blue);

	if (OutHitResult.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Right TraceCover Disabled Actor Name %s"),
		       *OutHitResult.GetActor()->GetName())
	}
	return Hit;
}

bool AMainCharacter::CrosshairTraceCoverDisable() {
	FVector OutStart;
	FVector OutEnd;
	TArray<AActor*> IgnoredActors;
	FHitResult HitResult;
	return UKismetSystemLibrary::LineTraceSingle(this,
	                                             OutStart,
	                                             OutEnd,
	                                             ETraceTypeQuery::TraceTypeQuery3,
	                                             false,
	                                             IgnoredActors,
	                                             EDrawDebugTrace::ForOneFrame,
	                                             HitResult,
	                                             false,
	                                             FLinearColor::Yellow,
	                                             FLinearColor::Blue);
}

bool AMainCharacter::LeftTraceCoverJumpBetweenCovers() {
	if (bCanPeakLeft) {
		FRotator NextCoverRotation = GetActorRotation();
		auto ControlRot = Controller->GetControlRotation();
		// NextCoverRotation.Normalize();
		CoverPitch = NextCoverRotation.Pitch;
		CoverRoll = NextCoverRotation.Roll;

		FHitResult NextCoverHitResult;
		FVector NextCoverTraceStart = CoverLeftMovement->GetComponentLocation();
		FVector RotFVector = -GetActorRotation().Quaternion().GetRightVector();
		FVector NextCoverTraceEnd = CoverLeftMovement->GetComponentLocation() + RotFVector * 150.f;
		TArray<AActor*> IgnoredActors = {CurrentCover};
		ETraceTypeQuery CoverTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
		UKismetSystemLibrary::LineTraceSingle(this,
		                                      NextCoverTraceStart,
		                                      NextCoverTraceEnd,
		                                      CoverTraceType,
		                                      false,
		                                      IgnoredActors,
		                                      EDrawDebugTrace::ForOneFrame,
		                                      NextCoverHitResult,
		                                      true,
		                                      FLinearColor::MakeRandomColor(),
		                                      FLinearColor::MakeRandomColor());

		if (NextCoverHitResult.bBlockingHit) {
			ACover* LeftTraceCover = Cast<ACover>(NextCoverHitResult.GetActor());
			if (LeftTraceCover) {
				SlideLeftCover = LeftTraceCover;

				FHitResult CoverMoveHitResult;
				FVector CoverMoveStart = NextCoverHitResult.Location;

				// float X = CoverRoll -.7f;
				// float Y = CoverPitch - 1.f;

				auto f = GetActorRotation().Quaternion().GetRightVector();
				auto x = GetActorRotation().Quaternion().GetForwardVector();

				float X = (FMath::RoundToInt(f.X) != 0.0f ? -f.X : -x.X) * 0.6;
				float Y = -(FMath::RoundToInt(f.Y) != 0.0f ? f.Y : x.Y) * 0.6;
				Y = (Y != 0.0f && Y != -0.0f ? Y : Y + 1.f);

				FVector CoverMoveRotFVector = {
					// -0.7f,
					X,
					// -1.f,
					Y,
					-1.0f
				};
				UE_LOG(LogTemp, Warning, TEXT("LEFT CoverMoveRotFVector: %s f: %s x: %s, ABS X: %s, ABS Y: %s"),
				       *CoverMoveRotFVector.ToString(), *f.ToString(), *x.ToString(),
				       FMath::RoundToInt(f.X) != 0.0f ? TEXT("true") : TEXT("false"),
				       FMath::RoundToInt(f.Y) != 0.0f ? TEXT("true") : TEXT("false"))

				FVector CoverMoveEnd = CoverMoveStart + CoverMoveRotFVector * 100.f;
				UKismetSystemLibrary::LineTraceSingle(this,
				                                      CoverMoveStart,
				                                      CoverMoveEnd,
				                                      CoverTraceType,
				                                      false,
				                                      IgnoredActors,
				                                      EDrawDebugTrace::ForOneFrame,
				                                      CoverMoveHitResult,
				                                      true,
				                                      FLinearColor::MakeRandomColor(),
				                                      FLinearColor::MakeRandomColor());

				if (CoverMoveHitResult.bBlockingHit) {
					// auto CoverPoint = SpawnCoverPoint(DefaultCoverPointClass);
					// CoverPoint->SetActorLocation(CoverMoveHitResult.Location);
					if (CoverPointEndTrace == nullptr)
						CoverPointEndTrace = SpawnCoverPoint(DefaultCoverPointClass);

					CoverPointEndTrace->SetActorLocation(CoverMoveHitResult.TraceEnd);
					CoverPointEndTrace->SetCoverNormal(CoverMoveHitResult.Normal);

				}
				else {
					if (CoverPointEndTrace != nullptr) {
						if (RemoveCoverPoint(CoverPointEndTrace)) {
							CoverPointEndTrace = nullptr;
						}
					}
				}

				return true;
			}

			if (CoverPointEndTrace != nullptr) {
				if (RemoveCoverPoint(CoverPointEndTrace)) {
					CoverPointEndTrace = nullptr;
				}
			}
		}
		else {
			if (CoverPointEndTrace != nullptr) {
				if (RemoveCoverPoint(CoverPointEndTrace)) {
					CoverPointEndTrace = nullptr;
				}
			}
		}
	}
	return false;
}

bool AMainCharacter::RightTraceCoverJumpBetweenCovers() {
	if (bCanPeakRight) {
		FRotator NextCoverRotation = GetActorRotation();
		auto ControlRot = Controller->GetControlRotation();
		// NextCoverRotation.Normalize();
		CoverPitch = NextCoverRotation.Pitch;
		CoverRoll = NextCoverRotation.Roll;

		FHitResult NextCoverHitResult;
		FVector NextCoverTraceStart = CoverRightMovement->GetComponentLocation();
		FVector RotFVector = UKismetMathLibrary::NegateVector(-GetActorRotation().Quaternion().GetRightVector());
		FVector NextCoverTraceEnd = CoverRightMovement->GetComponentLocation() + RotFVector * 150.f;
		TArray<AActor*> IgnoredActors = {CurrentCover};
		ETraceTypeQuery CoverTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
		UKismetSystemLibrary::LineTraceSingle(this,
		                                      NextCoverTraceStart,
		                                      NextCoverTraceEnd,
		                                      CoverTraceType,
		                                      false,
		                                      IgnoredActors,
		                                      EDrawDebugTrace::ForOneFrame,
		                                      NextCoverHitResult,
		                                      true,
		                                      FLinearColor::MakeRandomColor(),
		                                      FLinearColor::MakeRandomColor());

		if (NextCoverHitResult.bBlockingHit) {
			ACover* LeftTraceCover = Cast<ACover>(NextCoverHitResult.GetActor());
			if (LeftTraceCover) {
				SlideLeftCover = LeftTraceCover;

				FHitResult CoverMoveHitResult;
				FVector CoverMoveStart = NextCoverHitResult.Location;

				auto f = GetActorRotation().Quaternion().GetRightVector();
				auto x = GetActorRotation().Quaternion().GetForwardVector();

				float X = (FMath::RoundToInt(f.X) != 0.0f ? f.X : -x.X) * 0.6;
				float Y = (FMath::RoundToInt(f.Y) != 0.0f ? f.Y : -x.Y) * 0.6;
				Y = (Y != 0.0f && Y != -0.0f ? Y : Y + 1.f);

				FVector CoverMoveRotFVector = {
					// -0.7f,
					X,
					// 1.f,
					Y,
					-1.0f
				};
				UE_LOG(LogTemp, Warning, TEXT("RIGHT CoverMoveRotFVector: %s f: %s x: %s, ABS X: %s, ABS Y: %s"),
				       *CoverMoveRotFVector.ToString(), *f.ToString(), *x.ToString(),
				       FMath::RoundToInt(f.X) != 0.0f ? TEXT("true") : TEXT("false"),
				       FMath::RoundToInt(f.Y) != 0.0f ? TEXT("true") : TEXT("false"))

				FVector CoverMoveEnd = CoverMoveStart + CoverMoveRotFVector * 100.f;
				UKismetSystemLibrary::LineTraceSingle(this,
				                                      CoverMoveStart,
				                                      CoverMoveEnd,
				                                      CoverTraceType,
				                                      false,
				                                      IgnoredActors,
				                                      EDrawDebugTrace::ForOneFrame,
				                                      CoverMoveHitResult,
				                                      true,
				                                      FLinearColor::MakeRandomColor(),
				                                      FLinearColor::MakeRandomColor());

				if (CoverMoveHitResult.bBlockingHit) {
					// auto CoverPoint = SpawnCoverPoint(DefaultCoverPointClass);
					// CoverPoint->SetActorLocation(CoverMoveHitResult.Location);
					if (CoverPointEndTrace == nullptr)
						CoverPointEndTrace = SpawnCoverPoint(DefaultCoverPointClass);

					CoverPointEndTrace->SetActorLocation(CoverMoveHitResult.TraceEnd);
					CoverPointEndTrace->SetCoverNormal(CoverMoveHitResult.Normal);

				}
				else {
					if (CoverPointEndTrace != nullptr) {
						if (RemoveCoverPoint(CoverPointEndTrace)) {
							CoverPointEndTrace = nullptr;
						}
					}
				}

				return true;
			}

			if (CoverPointEndTrace != nullptr) {
				if (RemoveCoverPoint(CoverPointEndTrace)) {
					CoverPointEndTrace = nullptr;
				}
			}
		}
		else {
			if (CoverPointEndTrace != nullptr) {
				if (RemoveCoverPoint(CoverPointEndTrace)) {
					CoverPointEndTrace = nullptr;
				}
			}
		}
	}

	return false;
}

bool AMainCharacter::CoverTracer(UArrowComponent* AComponent, FHitResult& Result, float HalfHeight,
                                 ETraceTypeQuery TraceType, FLinearColor TraceColor, FLinearColor HitColor) {
	TArray<AActor*> IgnoredActors;
	FVector TraceStart = AComponent->GetComponentLocation();
	FVector TraceEnd = AComponent->GetComponentLocation();
	return UKismetSystemLibrary::CapsuleTraceSingle(this,
	                                                TraceStart,
	                                                TraceEnd,
	                                                20.0f,
	                                                HalfHeight,
	                                                TraceType,
	                                                false,
	                                                IgnoredActors,
	                                                EDrawDebugTrace::ForOneFrame,
	                                                Result,
	                                                true,
	                                                TraceColor,
	                                                HitColor
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
			                                            (GetActorRotation().Quaternion().GetRightVector() * -20.0f)
			                                            +
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

void AMainCharacter::MoveBetweenCovers() {
	UE_LOG(LogTemp, Warning, TEXT("MoveBetweenCovers %s"), bMoveInCover ? TEXT("true") : TEXT("false"))
	bMoveInCover = false;
}

void AMainCharacter::CoverMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	UE_LOG(LogTemp, Warning, TEXT("Montage End Name: %s"), *Montage->GetName());
	bCoverMontageEnded = true;
}

bool AMainCharacter::GetPlayerHUDVisibility() const {
	return !CharacterInventoryComponent->GetQuickSelectVisibility() && !CharacterInventoryComponent->
		GetInventoryVisible();
}

//Add listener on pause button click
bool AMainCharacter::GetPauseMenuVisibility() const {
	return bPauseMenuButtonPressed;
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

	InvisibleCameraHeadComponent->SetOwnerNoSee(!FollowCameraActive);
	InvisibleCameraHeadComponent->bCastHiddenShadow = !FollowCameraActive;
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

void AMainCharacter::PeakLeft_Implementation(bool PeakLeft) {
}

void AMainCharacter::PeakRight_Implementation(bool PeakRight) {
}

void AMainCharacter::PeakTop_Implementation(bool bPeakTop) {
}

float AMainCharacter::GetCrosshairSpreadMultiplier() const {
	return CrosshairSpreadMultiplier;
}
