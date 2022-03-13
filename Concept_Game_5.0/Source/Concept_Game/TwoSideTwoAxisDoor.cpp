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
			                                                : CurrentLeftRotYaw - RotationYawAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotYaw - RotationYawAngle
			                                                : CurrentLeftRotYaw,
		                                                CurrentDoorRotation);
		float RightLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotYaw
			                                                 : CurrentRightRotYaw + RotationYawAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotYaw + RotationYawAngle
			                                                 : CurrentRightRotYaw,
		                                                 CurrentDoorRotation);

		float LeftLerpRotPitch = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotPitch
			                                                : CurrentLeftRotPitch - RotationPitchAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotPitch - RotationPitchAngle
			                                                : CurrentLeftRotPitch,
		                                                CurrentDoorRotation);
		float RightLerpRotPitch = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotPitch
			                                                 : CurrentRightRotPitch + RotationPitchAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotPitch + RotationPitchAngle
			                                                 : CurrentRightRotPitch,
		                                                 CurrentDoorRotation);

		float LeftLerpRotRoll = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotRoll
			                                                : CurrentLeftRotRoll - RotationRollAngle,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotRoll - RotationRollAngle
			                                                : CurrentLeftRotRoll,
		                                                CurrentDoorRotation);
		float RightLerpRotRoll = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentRightRotRoll
			                                                 : CurrentRightRotRoll + RotationRollAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotRoll + RotationRollAngle
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
