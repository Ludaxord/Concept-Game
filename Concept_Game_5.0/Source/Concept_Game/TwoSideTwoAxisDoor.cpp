// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoSideTwoAxisDoor.h"

#include "Kismet/KismetMathLibrary.h"

ATwoSideTwoAxisDoor::ATwoSideTwoAxisDoor(): ATwoSideDoor() {
	ItemInteractionName = "Open";
	ItemName = "Door";
}

void ATwoSideTwoAxisDoor::Tick(float DeltaSeconds) {

	if (bIsOpenedRef != bIsOpened) {
		FRotator LRot = LeftDoorMesh->GetComponentRotation();
		FRotator RRot = RightDoorMesh->GetComponentRotation();

		float LeftLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotYaw
			                                                : CurrentLeftRotYaw - RotationYawLeftAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotYaw - RotationYawLeftAngle
			                                                : CurrentLeftRotYaw,
		                                                CurrentDoorRotation);
		float RightLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotYaw
			                                                 : CurrentRightRotYaw + RotationYawRightAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotYaw + RotationYawRightAngle
			                                                 : CurrentRightRotYaw,
		                                                 CurrentDoorRotation);

		float LeftLerpRotPitch = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotPitch
			                                                : CurrentLeftRotPitch - RotationPitchLeftAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotPitch - RotationPitchLeftAngle
			                                                : CurrentLeftRotPitch,
		                                                CurrentDoorRotation);
		float RightLerpRotPitch = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotPitch
			                                                 : CurrentRightRotPitch + RotationPitchRightAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotPitch + RotationPitchRightAngle
			                                                 : CurrentRightRotPitch,
		                                                 CurrentDoorRotation);

		float LeftLerpRotRoll = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotRoll
			                                                : CurrentLeftRotRoll - RotationRollLeftAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotRoll - RotationRollLeftAngle
			                                                : CurrentLeftRotRoll,
		                                                CurrentDoorRotation);
		float RightLerpRotRoll = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotRoll
			                                                 : CurrentRightRotRoll + RotationRollRightAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotRoll + RotationRollRightAngle
			                                                 : CurrentRightRotRoll,
		                                                 CurrentDoorRotation);

		FRotator LNewRot = {LeftLerpRotPitch, LeftLerpRotYaw, LeftLerpRotRoll};
		FRotator RNewRot = {RightLerpRotPitch, RightLerpRotYaw, RightLerpRotRoll};

		SetDoorRotation(LNewRot, RNewRot);
	}
}

void ATwoSideTwoAxisDoor::BeginPlay() {
	Super::BeginPlay();

	if (DoorMovementTransitionFloatCurve) {
		CurrentLeftRotPitch = LeftDoorMesh->GetComponentRotation().Pitch;
		CurrentRightRotPitch = RightDoorMesh->GetComponentRotation().Pitch;

		CurrentLeftRotRoll = LeftDoorMesh->GetComponentRotation().Roll;
		CurrentRightRotRoll = RightDoorMesh->GetComponentRotation().Roll;
	}
}
