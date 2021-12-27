// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterInputComponent.h"

#include "Cover.h"
#include "DebugWidgetComponent.h"
#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "MainHUD.h"
#include "MainPlayerController.h"
#include "PauseMenuComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMainCharacterInputComponent::UMainCharacterInputComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMainCharacterInputComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
}


// Called every frame
void UMainCharacterInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMainCharacterInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &UMainCharacterInputComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UMainCharacterInputComponent::MoveRight);
	PlayerInputComponent->BindAxis("ClimbRight", this, &UMainCharacterInputComponent::ClimbRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &UMainCharacterInputComponent::TurnRate);
	PlayerInputComponent->BindAxis("LookupRate", this, &UMainCharacterInputComponent::LookUpAtRate);
	PlayerInputComponent->BindAxis("PoseChange", this, &UMainCharacterInputComponent::ChangePoseAxisButtonPressed);
	PlayerInputComponent->BindAxis("Running", this, &UMainCharacterInputComponent::RunningButtonPressed);
	PlayerInputComponent->BindAxis("Turn", this, &UMainCharacterInputComponent::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &UMainCharacterInputComponent::LookUp);
	PlayerInputComponent->BindAxis("QuickSelect", this, &UMainCharacterInputComponent::QuickSelectButtonPressed);

	//NOTE, Debug bindings, remove in production
	PlayerInputComponent->BindAction("DebugChangeCamera", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::ChangeDebugCamera);
	PlayerInputComponent->BindAction("FPSCounterTrigger", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::TriggerFPSCounter);
	PlayerInputComponent->BindAction("DebugTriggerRotationYaw", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::ChangeDebugTriggerRotationYaw);

	PlayerInputComponent->BindAction("PoseChange", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::ChangePoseButtonPressed);
	PlayerInputComponent->BindAction("ClimbRightAction", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::ClimbRightActionPressed);
	PlayerInputComponent->BindAction("ClimbRightAction", IE_Released, this,
	                                 &UMainCharacterInputComponent::ClimbRightActionReleased);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UMainCharacterInputComponent::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &UMainCharacterInputComponent::StopJumping);
	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &UMainCharacterInputComponent::Cover);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &UMainCharacterInputComponent::AimingButtonPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &UMainCharacterInputComponent::AimingButtonReleased);
	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::UseWeaponButtonPressed);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this,
	                                 &UMainCharacterInputComponent::UseWeaponButtonReleased);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this,
	                                 &UMainCharacterInputComponent::InteractButtonPressed);
	PlayerInputComponent->BindAction("DebugDropItem", IE_Pressed, this, &UMainCharacterInputComponent::DebugDropItem);

	//TODO: Temporary moved to blueprint, if blueprint will work recreate system in cpp
	// PlayerInputComponent->BindAction("Inventory", IE_Pressed, this,&UMainCharacterInputComponent::InventoryButtonPressed);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &UMainCharacterInputComponent::PauseButtonPressed);
}

void UMainCharacterInputComponent::StopJumping() {
	OwningCharacter->StopJumping();
}

void UMainCharacterInputComponent::ClimbRight(float Value) {
	if (OwningCharacter->Controller != nullptr) {
		if (OwningCharacter->PoseType == EPoseType::EPT_Climb) {
			OwningCharacter->ClimbRightValue = Value;
			if (OwningCharacter->bClimbingButtonPressed) {
				if (OwningCharacter->bPlayClimbTurnAnimation)
					OwningCharacter->ClimbingTransitionTimeline->PlayFromStart();
			}
			else {
				if (OwningCharacter->bPlayClimbTurnAnimation)
					OwningCharacter->ClimbingTransitionTimeline->PlayFromStart();
			}
		}
	}
}

void UMainCharacterInputComponent::ClimbRightActionPressed() {
	OwningCharacter->bPlayClimbTurnAnimation = true;
	OwningCharacter->bClimbingButtonPressed = true;
}

void UMainCharacterInputComponent::ClimbRightActionReleased() {
	OwningCharacter->bPlayClimbTurnAnimation = true;
	OwningCharacter->bClimbingButtonPressed = false;
}

void UMainCharacterInputComponent::TurnRate(float Rate) {
	if (TurnRateBlocked(Rate)) {
		return;
	}
	OwningCharacter->TurnValue = Rate;
	OwningCharacter->AddControllerYawInput(Rate * OwningCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void UMainCharacterInputComponent::Turn(float Value) {
	if (TurnBlocked(Value)) {
		return;
	}
	if (OwningCharacter->PoseType == EPoseType::EPT_Climb) return;
	OwningCharacter->TurnVal = Value;
	if (OwningCharacter->bInCover || OwningCharacter->bCoverActive || OwningCharacter->bCoveringActive) return;
	float TurnScaleFactor = OwningCharacter->bAiming
		                        ? OwningCharacter->MouseAimingTurnRate
		                        : OwningCharacter->MouseHipTurnRate;
	OwningCharacter->AddControllerYawInput(Value * TurnScaleFactor);
}

void UMainCharacterInputComponent::LookUpAtRate(float Rate) {
	if (LookUpAtRateBlocked(Rate)) {
		return;
	}
	OwningCharacter->LookValue = Rate;
	OwningCharacter->AddControllerPitchInput(Rate * OwningCharacter->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UMainCharacterInputComponent::LookUp(float Value) {
	if (LookUpBlocked(Value)) {
		return;
	}
	if (OwningCharacter->PoseType == EPoseType::EPT_Climb) return;
	OwningCharacter->LookUpVal = Value;
	if (OwningCharacter->bInCover || OwningCharacter->bCoverActive || OwningCharacter->bCoveringActive) return;
	float LookUpScaleFactor = OwningCharacter->bAiming
		                          ? OwningCharacter->MouseAimingLookUpRate
		                          : OwningCharacter->MouseHipLookUpRate;
	OwningCharacter->AddControllerPitchInput(Value * LookUpScaleFactor);
}

void UMainCharacterInputComponent::Cover() {
	if (!OwningCharacter->bCoverActive) {
		if (!OwningCharacter->bCoverDisable) {
			FVector OutStart;
			FVector OutEnd;
			FHitResult Res;
			OwningCharacter->GetForwardTracers(OutStart, OutEnd, Res);

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
				if (OutHitResult.bBlockingHit) {
					OwningCharacter->CurrentCoverHitResult = OutHitResult;

					OutHitResult.Actor->GetActorBounds(false, OwningCharacter->CurrentCoverOrigin,
					                                   OwningCharacter->CurrentCoverBoxExtend);
					OwningCharacter->CoverLocation = OutHitResult.Location;
					OwningCharacter->CoverNormal = OutHitResult.Normal;
					OwningCharacter->CurrentCover = Cast<ACover>(OutHitResult.Actor);
					if (OwningCharacter->CurrentCover) {
						OwningCharacter->CurrentCover->SetCurrentOverlappingCharacter(OwningCharacter);
						OwningCharacter->CurrentCover->TakeCover();
						//TODO: Add anim montage And rotation Root Yaw.
						OwningCharacter->bCoveringActive = true;
					}
				}
			}
		}
	}
	else {
		if (OwningCharacter->bInCover) {
			OwningCharacter->CurrentCover->QuitCover();
			OwningCharacter->bCoveringActive = false;
			OwningCharacter->CurrentCoverHitResult = FHitResult();
			if (OwningCharacter->CurrentCoverPoint != nullptr) {
				if (OwningCharacter->RemoveCoverPoint(OwningCharacter->CurrentCoverPoint)) {
					OwningCharacter->CurrentCoverPoint = nullptr;
				}
			}

			OwningCharacter->CurrentCover = nullptr;
			// PlayMontage(ECharacterMontage::ECM_ExitCover);
			//TODO: Add anim montage And rotation Root Yaw.
		}
	}
	if (!OwningCharacter->bCoverDisable) {
		OwningCharacter->bCoverActive = !OwningCharacter->bCoverActive;
	}

	UE_LOG(LogTemp, Warning, TEXT("CoverDisable: %s CoverActive: %s InCover: %s CanCover: %s"),
	       OwningCharacter->bCoverDisable ? TEXT("true") : TEXT("false"),
	       OwningCharacter->bCoverActive ? TEXT("true") : TEXT("false"),
	       OwningCharacter->bInCover ? TEXT("true") : TEXT("false"),
	       OwningCharacter->bCanCover ? TEXT("true") : TEXT("false"));
}

void UMainCharacterInputComponent::AimingButtonPressed() {
	if (AimButtonPressedBlocked()) {
		return;
	}

	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	OwningCharacter->CrossHairSpreadMulti = HUD->GetCrossHairSpreadMultiplier();
	OwningCharacter->CrossHairSpreadMax = HUD->GetCrossHairSpreadMax();
	OwningCharacter->CrossHairHalfHeight = HUD->GetCrossHairHalfHeight();
	OwningCharacter->CrossHairHalfWidth = HUD->GetCrossHairHalfWidth();
	OwningCharacter->ScreenCenter = HUD->GetScreenCenter();
	OwningCharacter->CrossHairLocation = HUD->GetCrossHairLocation();
	OwningCharacter->CrossHairBaseCenter = HUD->GetCrossHairBaseCenter();

	OwningCharacter->bAimingButtonPressed = true;
	if (OwningCharacter->CombatState != ECombatState::ECS_Reloading && OwningCharacter->CombatState !=
		ECombatState::ECS_Equipping &&
		OwningCharacter->CombatState != ECombatState::ECS_Stunned) {
		if (!OwningCharacter->bInCover && !OwningCharacter->bCoverActive && !OwningCharacter->bCoveringActive) {
			OwningCharacter->Aim();
		}
		else {
			if (OwningCharacter->CurrentCover)
				OwningCharacter->CurrentCover->Aim();
		}
	}
}

void UMainCharacterInputComponent::AimingButtonReleased() {
	if (AimButtonReleasedBlocked()) {
		return;
	}

	OwningCharacter->bAimingButtonPressed = false;
	if (!OwningCharacter->bInCover && !OwningCharacter->bCoverActive && !OwningCharacter->bCoveringActive) {
		OwningCharacter->StopAiming();
	}
	else {
		if (OwningCharacter->CurrentCover)
			OwningCharacter->CurrentCover->StopAiming();
	}
}

void UMainCharacterInputComponent::RunningButtonPressed(float Value) {
	OwningCharacter->bRunning = (OwningCharacter->PoseType == EPoseType::EPT_Stand && Value > 0);
	// UE_LOG(LogTemp, Warning, TEXT("Running %s"), bRunning ? TEXT("true") : TEXT("false"))
}

void UMainCharacterInputComponent::RunningButtonReleased() {
	if (OwningCharacter->PoseType == EPoseType::EPT_Stand)
		OwningCharacter->bRunning = false;
}

void UMainCharacterInputComponent::ChangePoseButtonPressed(FKey Key) {
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	auto KeyTime = Player->GetInputKeyTimeDown(Key);

	switch (OwningCharacter->PoseType) {
	case EPoseType::EPT_Stand:
		OwningCharacter->Crouching();
		break;
	case EPoseType::EPT_Crouch: {
		OwningCharacter->Standing();
	}
	break;
	case EPoseType::EPT_Crawl:
		OwningCharacter->Crouching();
		break;
	case EPoseType::EPT_Run:
		break;
	case EPoseType::EPT_Aim:
		break;
	case EPoseType::EPT_Climb:
		OwningCharacter->Climbing();
		break;
	case EPoseType::EPT_MAX: break;
	default:
		OwningCharacter->Standing();
		break;
	}

	const TEnumAsByte<EPoseType> PoseEnum = OwningCharacter->PoseType;
	FString EnumAsString = UEnum::GetValueAsString(PoseEnum.GetValue());

	UE_LOG(LogTemp, Warning, TEXT("Pose: %s, Pressed Key %s, Key Time %f Key Value"), *EnumAsString,
	       *(Key.GetDisplayName().ToString()), KeyTime)
}

void UMainCharacterInputComponent::ChangePoseAxisButtonPressed(float Value) {
	if (OwningCharacter->PoseAxisValueCounter > 80) {
		OwningCharacter->Crawling();
	}

	if (Value == 0)
		OwningCharacter->PoseAxisValueCounter = 0;
	else
		OwningCharacter->PoseAxisValueCounter += Value;
}

void UMainCharacterInputComponent::Jump() {
	UE_LOG(LogTemp, Warning, TEXT("Jump from char"))
	OwningCharacter->Jump();
}

void UMainCharacterInputComponent::UseWeaponButtonPressed() {
	UE_LOG(LogTemp, Warning, TEXT("UseWeaponButtonPressed"));
	if (UseWeaponPressedBlocked()) {
		return;
	}
	OwningCharacter->bUseWeaponButtonPressed = true;
	if (OwningCharacter->bInCover) {
		if (OwningCharacter->bCanPeakLeft || OwningCharacter->bCanPeakRight || OwningCharacter->bCanPeakTop) {
			if (!OwningCharacter->bAimingButtonPressed) {
				if (OwningCharacter->bCanPeakLeft) {
					OwningCharacter->bCoverPeakLeft = true;
				}
				OwningCharacter->PeakLeft();

				if (OwningCharacter->bCanPeakRight) {
					OwningCharacter->bCoverPeakRight = true;
				}
				OwningCharacter->PeakRight();

				if (!OwningCharacter->bCoverPeakRight && !OwningCharacter->bCoverPeakLeft) {
					if (OwningCharacter->bCanPeakTop) {
						OwningCharacter->bCoverPeakTop = true;
					}
					OwningCharacter->PeakTop();
				}
			}
			OwningCharacter->UseWeapon();
		}
	}
	else {
		OwningCharacter->UseWeapon();
	}
}

void UMainCharacterInputComponent::UseWeaponButtonReleased() {
	UE_LOG(LogTemp, Warning, TEXT("UseWeaponButtonReleased"));
	if (UseWeaponReleasedBlocked()) {
		return;
	}
	OwningCharacter->bUseWeaponButtonPressed = false;
	if (!OwningCharacter->bAimingButtonPressed) {
		if (OwningCharacter->bCanPeakLeft || OwningCharacter->bCanPeakRight || OwningCharacter->bCanPeakTop) {
			OwningCharacter->bCoverPeakLeft = false;
			OwningCharacter->PeakLeft();
			OwningCharacter->bCoverPeakRight = false;
			OwningCharacter->PeakRight();
			OwningCharacter->bCoverPeakTop = false;
			OwningCharacter->PeakTop();
		}
	}
}

void UMainCharacterInputComponent::ChangeDebugCamera() {
	if (!OwningCharacter->GetCharacterMovement()->IsFalling()) {
		OwningCharacter->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		OwningCharacter->EyesCameraTransform = OwningCharacter->SetCameraTransform(
			OwningCharacter->EyesCamera, "head", true, OwningCharacter->GetMesh());
		OwningCharacter->bSwitchToFollowCamera = !OwningCharacter->bSwitchToFollowCamera;
		UE_LOG(LogTemp, Error, TEXT("Camera TPP: %s"),
		       OwningCharacter->bSwitchToFollowCamera ? TEXT("true") : TEXT("false"))

		if (!OwningCharacter->bSwitchToFollowCamera) {
			OwningCharacter->FollowCameraTransform = OwningCharacter->SetCameraTransform(OwningCharacter->FollowCamera);
			OwningCharacter->ChangeCameraTimeline->PlayFromStart();

			OwningCharacter->bUseControllerRotationPitch = false;
			OwningCharacter->bUseControllerRotationYaw = true;
			OwningCharacter->bUseControllerRotationRoll = false;
		}
		else {
			OwningCharacter->SetActiveCameras(true);
			OwningCharacter->FollowCameraTransform = OwningCharacter->RefFollowCamera->GetComponentTransform();
			OwningCharacter->ChangeCameraTimeline->ReverseFromEnd();
			//TODO: Fix animation when change controller rotation Yaw to false;
			OwningCharacter->bUseControllerRotationPitch = false;
			OwningCharacter->bUseControllerRotationYaw = OwningCharacter->bRotationYaw;
			OwningCharacter->bUseControllerRotationRoll = false;

		}
	}
}

void UMainCharacterInputComponent::ChangeDebugTriggerRotationYaw() {
	OwningCharacter->bRotationYaw = !OwningCharacter->bRotationYaw;
	UE_LOG(LogTemp, Warning, TEXT("Change Debug Trigger Rotation Yaw %s"),
	       OwningCharacter->bRotationYaw ? TEXT("true") : TEXT("false"));
}

void UMainCharacterInputComponent::TriggerFPSCounter() {
	OwningCharacter->bFPSCounter = !OwningCharacter->bFPSCounter;
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "stat fps");
}

void UMainCharacterInputComponent::InteractButtonPressed() {
	if (OwningCharacter->CombatState != ECombatState::ECS_Unoccupied) return;
	if (OwningCharacter->TraceHitItem) {
		OwningCharacter->TraceHitItem->InteractWithItem(OwningCharacter);
		OwningCharacter->TraceHitItem = nullptr;
	}
}

void UMainCharacterInputComponent::InventoryButtonPressed() {
	OwningCharacter->CharacterInventoryComponent->InventoryToggle();
}

void UMainCharacterInputComponent::DebugWidgetButtonPressed() {
	OwningCharacter->DebugWidgetComponent->Toggle();
}

void UMainCharacterInputComponent::PauseButtonPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Pause Button"));
	if (!OwningCharacter->CharacterInventoryComponent->QuitActionButtonPressed()) {
		OwningCharacter->bPauseMenuButtonPressed = !OwningCharacter->bPauseMenuButtonPressed;
		OwningCharacter->CharacterPauseMenuComponent->PauseMenuToggle();
	}
}

void UMainCharacterInputComponent::DebugDropItem() {
	OwningCharacter->DropItem(OwningCharacter->EquippedWeapon);
	OwningCharacter->SetEquippedWeapon(nullptr);
	// UE_LOG(LogTemp, Warning, TEXT("Item Dropped"));
}

void UMainCharacterInputComponent::QuickSelectButtonPressed(float Value) {
	// CharacterInventoryComponent->QuickSelectToggle(Value > 0.0f);
	OwningCharacter->CharacterInventoryComponent->QuickSelectPieToggle(Value > 0.0f);
}

bool UMainCharacterInputComponent::AimButtonPressedBlocked() {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		UE_LOG(LogTemp, Warning, TEXT("AIM BUTTON BLOCKED"))
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::AimButtonReleasedBlocked() {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::UseWeaponPressedBlocked() {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		OwningCharacter->CharacterInventoryComponent->MouseButtonPressed();
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::UseWeaponReleasedBlocked() {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::TurnRateBlocked(float Rate) {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::TurnBlocked(float Value) {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::LookUpAtRateBlocked(float Rate) {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

bool UMainCharacterInputComponent::LookUpBlocked(float Value) {
	if (OwningCharacter->CharacterInventoryComponent->GetQuickSelectVisibility()) {
		return true;
	}

	if (OwningCharacter->CharacterInventoryComponent->GetInventoryVisible()) {
		return true;
	}

	if (OwningCharacter->CharacterPauseMenuComponent->GetPauseMenuVisibility()) {
		return true;
	}

	return false;
}

void UMainCharacterInputComponent::MoveForward(float Value) {
	if (OwningCharacter->Controller != nullptr && (Value != 0.0f)) {
		FVector Direction;
		if (!OwningCharacter->bInCover || ! OwningCharacter->bCoverActive || !OwningCharacter->bCoveringActive) {
			const FRotator Rotation = OwningCharacter->Controller->GetControlRotation();
			const FRotator YawRotation = {0, Rotation.Yaw, 0};
			if (OwningCharacter->PoseType == EPoseType::EPT_Climb) {
				Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
				if (Value < 0.0f) {
					if (OwningCharacter->GetOverlappingLadderBottom() && OwningCharacter->bTouchingFloor) {
						OwningCharacter->PoseType = EPoseType::EPT_Stand;
						OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = OwningCharacter->BaseMovementSpeed;
						OwningCharacter->SwitchCamera(false);
					}
				}
				else if (Value > 0.0f) {
					if (OwningCharacter->GetOverlappingLadderTop()) {

					}
				}
			}
			else {
				Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			}
		}
		else {
			if (OwningCharacter->CurrentCover)
				OwningCharacter->CurrentCover->MoveForward(Value);
		}

		OwningCharacter->AddMovementInput(Direction, Value);
	}
}

void UMainCharacterInputComponent::MoveRight(float Value) {
	OwningCharacter->MoveRightValue = Value;
	if (OwningCharacter->Controller != nullptr && (Value != 0.0f)) {
		if (OwningCharacter->PoseType != EPoseType::EPT_Climb && !OwningCharacter->bInCover) {
			const FRotator Rotation = OwningCharacter->Controller->GetControlRotation();
			const FRotator YawRotation = {0, Rotation.Yaw, 0};
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			OwningCharacter->AddMovementInput(Direction, Value);
			OwningCharacter->ClimbStartRotationYaw = OwningCharacter->Controller->GetControlRotation().Yaw;
			OwningCharacter->ClimbStartRotationYawLeft = OwningCharacter->ClimbStartRotationYaw + 45.0f;
			OwningCharacter->ClimbStartRotationYawRight = OwningCharacter->ClimbStartRotationYaw - 45.0f;
		}
		else if (OwningCharacter->bInCover) {
			if (OwningCharacter->CurrentCover)
				OwningCharacter->CurrentCover->MoveRight(Value);
		}
	}
}
