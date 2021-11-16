// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterCameraManager.h"

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMainCharacterCameraManager::AMainCharacterCameraManager(): SwayYawIntensity(3.0f), SwayPitchIntensity(3.0f) {
	MainCharacter = Cast<AMainCharacter>(ViewTarget.GetTargetPawn());
}

void AMainCharacterCameraManager::DoUpdateCamera(float DeltaTime) {
	Super::DoUpdateCamera(DeltaTime);
}

void AMainCharacterCameraManager::UpdateCamera(float DeltaTime) {
	Super::UpdateCamera(DeltaTime);
}

void AMainCharacterCameraManager::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (MainCharacter) {
		if (MainCharacter->GetCameraState() == ECameraState::ECS_EyesCamera) {
			SwayOffset = UKismetMathLibrary::VInterpTo(SwayOffset, FVector(0.0f), DeltaSeconds, 8.0f);
			SwayOffset = UKismetMathLibrary::VInterpTo(SwayOffset,
			                                           MainCharacter->GetAimingButtonPressed()
				                                           ? SwayAim()
				                                           : SwayIdle(),
			                                           DeltaSeconds,
			                                           5.0f);
		}
	}

	
}

FVector AMainCharacterCameraManager::SwayIdle() {
	FVector SwayIdleVector;
	if (MainCharacter) {
		SwayIdleVector = SwayOffset + FVector(
			0.0f,
			UKismetMathLibrary::FClamp(MainCharacter->GetTurnValue() * SwayYawIntensity, -20.0f, 20.0f),
			UKismetMathLibrary::FClamp(MainCharacter->GetLookValue() * SwayPitchIntensity, -10.0f, 10.0f)
		);
	}

	return SwayIdleVector;
}

FVector AMainCharacterCameraManager::SwayAim() {
	FVector SwayAimVector;
	if (MainCharacter) {
		SwayAimVector = SwayOffset + FVector(
			0.0f,
			UKismetMathLibrary::FClamp(MainCharacter->GetTurnValue() * SwayYawIntensity, -0.7f, 0.7f),
			UKismetMathLibrary::FClamp(MainCharacter->GetLookValue() * SwayPitchIntensity * -1.0f, -0.7f, 0.7f)
		);
	}

	return SwayAimVector;
}

void AMainCharacterCameraManager::UpdateCameraProperties() {
	MainCharacter = Cast<AMainCharacter>(ViewTarget.GetTargetPawn());

	if (!MainCharacter) return;
	if (!MainCharacter->GetEquippedWeapon()) return;
	if (MainCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb) return;


	UE_LOG(LogTemp, Warning, TEXT("Update Camera"));

	FinalCameraRotation = MainCharacter->GetControlRotation();

	FTransform SocketTransform = MainCharacter->GetEquippedWeapon()->GetItemMesh()->GetSocketTransform(
		FName("Ironsight"));
	FVector CameraLocation = MainCharacter->GetEyesCamera()->GetComponentLocation();
	FVector CameraSocketDelta = SocketTransform.GetLocation() - CameraLocation;
	FVector NormalizedLocation = UKismetMathLibrary::Normal(CameraSocketDelta, 0.0f);
	FVector SocketForwardVector = SocketTransform.GetRotation().GetForwardVector();
	float DotProductLocation = UKismetMathLibrary::Dot_VectorVector(NormalizedLocation,
	                                                                SocketForwardVector);
	float BProduct = DotProductLocation * (CameraSocketDelta.Size() / 250.f);
	FinalCameraLocation =
		UKismetMathLibrary::Quat_RotateVector(FinalCameraRotation.Quaternion(), SwayOffset) +
		UKismetMathLibrary::VLerp(
			CameraLocation,
			(SocketForwardVector * BProduct) - SocketTransform.GetLocation(),
			MainCharacter->GetCurrentAimValue()
		);

	FinalCameraFOV = UKismetMathLibrary::Lerp(90.0f, 80.0f, MainCharacter->GetCurrentAimValue());
}
