// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMainAnimInstance::UMainAnimInstance(): CurrentAttackType(EAttackType::EAT_RightHandFireWeapon),
                                        ActionState(EActionState::EAS_Hip),
                                        bIsInAir(false),
                                        bIsAccelerating(false),
                                        bRunning(false),
                                        bItemEquipped(false),
                                        bClimbing(false),
                                        bAiming(false),
                                        bReloading(false),
                                        bOverlappingLadderTop(false),
                                        bOverlappingLadderBottom(false),
                                        bTurnInPlace(false),
                                        PoseType(EPoseType::EPT_Stand),
                                        MovementOffsetYaw(0.0f),
                                        RecoilWeight(1.0f),
                                        LastMovementOffsetYaw(0.0f),
                                        RootYawOffset(0.0f),
                                        Yaw(0.0f),
                                        Pitch(0.0f),
                                        Speed(0.0f) {

}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime) {
	if (MainCharacter == nullptr)
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (MainCharacter) {
		PoseType = MainCharacter->GetCurrentPoseType();
		bReloading = MainCharacter->GetCombatState() == ECombatState::ECS_Reloading;
		bEquipping = MainCharacter->GetCombatState() == ECombatState::ECS_Equipping;
		bClimbing = MainCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb;
		bItemEquipped = MainCharacter->GetEquippedWeapon() != nullptr;
		bJumpFromClimb = MainCharacter->GetJumpFromClimb();
		bInCover = MainCharacter->GetInCover();
		UE_LOG(LogTemp, Warning, TEXT("Animation In Cover %s"), bInCover ? TEXT("true") : TEXT("false"));

		const TEnumAsByte<EPoseType> PoseEnum = MainCharacter->GetCurrentPoseType();
		FString PoseEnumAsString = UEnum::GetValueAsString(PoseEnum.GetValue());

		//NOTE: Temporary, detect if player use ladder from bottom or from top
		bOverlappingLadderBottom = MainCharacter->GetOverlappingLadderBottom();
		bOverlappingLadderTop = MainCharacter->GetOverlappingLadderTop();

		bCrawling = MainCharacter->GetCurrentPoseType() == EPoseType::EPT_Crawl;
		bCrouching = MainCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb;
		bRunning = MainCharacter->GetRunning();
		// UE_LOG(LogTemp, Warning, TEXT("Running: %s"), bRunning ? TEXT("true") : TEXT("false"));
		//TODO: Add FABRIK

		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		FVector VelocityLevel = MainCharacter->GetVelocity();
		VelocityLevel.X = 0;
		VelocityLevel.Y = 0;
		if (VelocityLevel.Z > 0) {
			LevelChangeSpeed = UKismetMathLibrary::NormalizeToRange(VelocityLevel.Size(),
			                                                        0,
			                                                        MainCharacter->GetCharacterMovement()->
			                                                        GetMaxSpeed());
		}
		else {
			LevelChangeSpeed = -UKismetMathLibrary::NormalizeToRange(VelocityLevel.Size(),
			                                                         0,
			                                                         MainCharacter->GetCharacterMovement()->
			                                                         GetMaxSpeed());
		}


		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();
		if (MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0) {
			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}

		FRotator AimRotation = MainCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (MainCharacter->GetVelocity().Size() > 0) {
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = MainCharacter->GetAiming();

		if (bReloading) {
			ActionState = EActionState::EAS_Reloading;
		}
		else if (bIsInAir) {
			ActionState = EActionState::EAS_InAir;
		}
		else if (MainCharacter->GetAiming()) {
			ActionState = EActionState::EAS_Aiming;
		}
		else if (bClimbing) {
			ActionState = EActionState::EAS_Climbing;
		}
		else {
			ActionState = EActionState::EAS_Hip;
		}

		//TODO: Add Equipped Weapon Type
		if (MainCharacter->GetEquippedWeapon()) {

		}

		AimOffsets(DeltaTime);
		TurnInPlace();
		SnapLeftHandToWeapon();
		WeaponSway(DeltaTime);
		// Lean(DeltaTime);
	}
}

void UMainAnimInstance::NativeInitializeAnimation() {
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::TurnInPlace() {
	if (MainCharacter == nullptr) return;

	LastYawInPlace = YawInPlace;

	float LocalPitch = 0;
	float LocalRoll = 0;
	float LocalYaw = 0;

	UKismetMathLibrary::BreakRotator(TryGetPawnOwner()->GetControlRotation(), LocalRoll, LocalPitch, LocalYaw);
	YawInPlace = LocalYaw;

	YawDelta = LastYawInPlace - YawInPlace;
	RootYawOffset = IsAnyMontagePlaying() || bIsAccelerating
		                ? 0.0f
		                : UKismetMathLibrary::NormalizeAxis(YawDelta + RootYawOffset);
	float CValue = 0.0f;
	bool bExists = GetCurveValue(FName("IsTurning"), CValue);
	bool CurveEqual = UKismetMathLibrary::NearlyEqual_FloatFloat(CValue, 1.0f, 0.001f);
	if (CurveEqual) {
		if (bCurveEqual) {
			DistanceCurveValue = RootYawOffset;
			bCurveEqual = false;
		}
		DistanceCurveValueLastFrame = RootYawOffset;
		bExists = GetCurveValue(FName("DistanceCurve"), CValue);

		DistanceCurveValue = UKismetMathLibrary::FClamp(CValue, -90.f, 0.f);
		RootYawOffset = RootYawOffset - ((DistanceCurveValueLastFrame - DistanceCurveValue) *
			UKismetMathLibrary::SelectFloat(-1.0f, 1.0f, RootYawOffset > 0));
	}
	else {
		bCurveEqual = true;
	}

}

void UMainAnimInstance::AimOffsets(float DeltaTime) {
	FRotator PlayerControlRotation = MainCharacter->GetControlRotation();
	FRotator PlayerActorRotation = MainCharacter->GetActorRotation();
	FRotator DeltaRotation = PlayerControlRotation - PlayerActorRotation;
	FRotator Rot = UKismetMathLibrary::MakeRotator(0.0f, Pitch, RootYawOffset);
	FRotator InterpRot = UKismetMathLibrary::RInterpTo(Rot, DeltaRotation, DeltaTime, 10.0f);
	float InterpRotRoll = 0.0f;
	float InterpRotPitch = 0.0f;
	float InterpRotYaw = 0.0f;
	UKismetMathLibrary::BreakRotator(InterpRot, InterpRotRoll, InterpRotPitch, InterpRotYaw);
	Yaw = UKismetMathLibrary::ClampAngle(InterpRotYaw, -90.0f, 90.0f);
	Pitch = UKismetMathLibrary::ClampAngle(InterpRotPitch, -90.0f, 90.0f);
}

void UMainAnimInstance::Lean(float DeltaTime) {
	if (MainCharacter == nullptr) return;
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MainCharacter->GetActorRotation();

	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.0f);

	YawDelta = FMath::Clamp(Interp, -90.0f, 90.0f);
}

void UMainAnimInstance::SnapLeftHandToWeapon() {
	if (MainCharacter == nullptr) return;

	if (MainCharacter->GetEquippedWeapon()) {
		USkeletalMeshComponent* WeaponMeshComponent = MainCharacter->GetEquippedWeapon()->GetItemMesh();
		USkeletalMeshComponent* PlayerMeshComponent = MainCharacter->GetMesh();
		FTransform LeftHandPosition = WeaponMeshComponent->GetSocketTransform("LeftHandSocket");
		FVector OutPosition;
		FRotator OutRotation;
		PlayerMeshComponent->TransformToBoneSpace(FName("hand_r"), LeftHandPosition.GetLocation(),
		                                          LeftHandPosition.GetRotation().Rotator(), OutPosition, OutRotation);
		LeftHandTransform = FTransform(OutRotation, OutPosition);
	}
}

void UMainAnimInstance::WeaponSway(float DeltaTime) {

	if (MainCharacter == nullptr) return;

	if (MainCharacter->GetEquippedWeapon()) {
		FRotator RightHandRot;
		if (MainCharacter->GetAimingButtonPressed()) {
			RightHandRot = FRotator(0.0f);
		}
		else {
			float WeaponPitch = FMath::Clamp(MainCharacter->GetMoveRightValue() * 10.0f, -7.0f, 7.0f);
			RightHandRot = FRotator(WeaponPitch, 0.0f, 0.0f);

		}
		RightHandRotation = UKismetMathLibrary::RInterpTo(RightHandRotation, RightHandRot, DeltaTime, 6.0f);
	}
}

void UMainAnimInstance::CanCover_Implementation(bool bCanCover) {

}
