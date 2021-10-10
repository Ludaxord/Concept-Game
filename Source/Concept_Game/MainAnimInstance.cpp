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
                                        bAiming(false),
                                        bReloading(false),
                                        bTurnInPlace(false),
                                        bCrouching(false),
                                        bCrawling(false),
                                        MovementOffsetYaw(0.0f),
                                        LastMovementOffsetYaw(0.0f),
                                        RootYawOffset(0.0f),
                                        Speed(0.0f) {

}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime) {
	if (MainCharacter == nullptr)
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (MainCharacter) {
		bCrouching = MainCharacter->GetCrouching();
		bReloading = MainCharacter->GetCombatState() == ECombatState::ECS_Reloading;
		bEquipping = MainCharacter->GetCombatState() == ECombatState::ECS_Equipping;
		//TODO: Add FABRIK

		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

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
		else {
			ActionState = EActionState::EAS_Hip;
		}

		//TODO: Add Equipped Weapon Type
		if (MainCharacter->GetEquippedWeapon()) {

		}

		TurnInPlace();
		Lean(DeltaTime);
	}
}

void UMainAnimInstance::NativeInitializeAnimation() {
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::TurnInPlace() {
	if (MainCharacter == nullptr) return;

	Pitch = MainCharacter->GetBaseAimRotation().Pitch;

	if (Speed > 0 || bIsInAir) {
		RootYawOffset = 0.0f;
		TIPCharacterYaw = MainCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurve = 0.0f;
		RotationCurveValueLastFrame = RotationCurve;
	}
	else {
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = MainCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta = TIPCharacterYaw - TIPCharacterYawLastFrame;

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

		const float Turning = GetCurveValue(TEXT("Turning"));
		if (Turning > 0) {
			bTurnInPlace = true;
			RotationCurveValueLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));
			const float DeltaRotation = RotationCurve - RotationCurveValueLastFrame;
			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			const float ABSRootYawOffset = FMath::Abs(RootYawOffset);
			if (ABSRootYawOffset > 90.0f) {
				const float YawExcess = ABSRootYawOffset - 90.0f;
				RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
		else {
			bTurnInPlace = false;
		}
	}

	RecoilWeight = bTurnInPlace
		               ? (bReloading || bEquipping ? 1.0f : 0.0f)
		               : (bCrouching
			                  ? (bReloading || bEquipping ? 1.0f : 0.1f)
			                  : (bAiming || bReloading || bEquipping ? 1.0f : 0.5f));
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
