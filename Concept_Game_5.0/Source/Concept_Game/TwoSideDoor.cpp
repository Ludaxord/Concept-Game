// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoSideDoor.h"

#include "Kismet/KismetMathLibrary.h"

ATwoSideDoor::ATwoSideDoor(): ADoor(), RotationYawLeftAngle(RotationYawAngle), RotationYawRightAngle(RotationYawAngle) {
	ItemInteractionName = "Open";
	ItemName = "Door";

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));

	LeftDoorMesh->SetupAttachment(PhysicsBasedMesh);
	RightDoorMesh->SetupAttachment(PhysicsBasedMesh);

	LeftDoorMesh->SetSimulatePhysics(false);
	LeftDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	RightDoorMesh->SetSimulatePhysics(false);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

}

void ATwoSideDoor::BeginPlay() {
	Super::BeginPlay();

	if (DoorMovementTransitionFloatCurve) {
		CurrentLeftRotYaw = LeftDoorMesh->GetComponentRotation().Yaw;
		CurrentRightRotYaw = RightDoorMesh->GetComponentRotation().Yaw;
	}
}

void ATwoSideDoor::Tick(float DeltaSeconds) {
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
			                                                 : CurrentLeftRotYaw + RotationYawRightAngle,
		                                                 bIsOpenedRef
			                                                 ? CurrentRightRotYaw + RotationYawRightAngle
			                                                 : CurrentRightRotYaw,
		                                                 CurrentDoorRotation);

		FRotator LNewRot = {LRot.Pitch, LeftLerpRotYaw, LRot.Roll};
		FRotator RNewRot = {RRot.Pitch, RightLerpRotYaw, RRot.Roll};

		SetDoorRotation(LNewRot, RNewRot);
	}
}

void ATwoSideDoor::SetDoorRotation(FRotator LeftRotator, FRotator RightRotator) {
	LeftDoorMesh->SetWorldLocationAndRotation(LeftDoorMesh->GetComponentLocation(), LeftRotator);
	RightDoorMesh->SetWorldLocationAndRotation(RightDoorMesh->GetComponentLocation(), RightRotator);
	if (CurrentDoorRotation == 1.f) {
		bIsOpened = !bIsOpened;
		ItemInteractionName = bIsOpened ? "Close" : "Open";
		CurrentDoorRotation = 0;
	}
}
