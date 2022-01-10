// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabinet.h"

#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ACabinet::ACabinet(): bIsOpened(false) {
	ItemInteractionName = "Open";
	ItemName = "Cabinet";

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));

	LeftDoorMesh->SetupAttachment(PhysicsBasedMesh);
	RightDoorMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftDoorPhysicsConstraint"));
	RightDoorPhysicsConstraint = CreateDefaultSubobject<
		UPhysicsConstraintComponent>(TEXT("RightDoorPhysicsConstraint"));

	DoorMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorMovementTransitionTimeline"));

	// LeftDoorPhysicsConstraint->SetupAttachment(LeftDoorMesh);
	// RightDoorPhysicsConstraint->SetupAttachment(RightDoorMesh);

	// LeftDoorPhysicsConstraint->ComponentName1 = {"LeftDoorMesh"};
	// LeftDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};

	// RightDoorPhysicsConstraint->ComponentName1 = {"RightDoorMesh"};
	// RightDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};
}

void ACabinet::BeginPlay() {
	Super::BeginPlay();

	DoorMovementFunctionFloat.BindDynamic(this, &ACabinet::UpdateDoorMovementTransitionTimeline);
	if (DoorMovementTransitionFloatCurve) {
		DoorMovementTransitionTimeline->AddInterpFloat(DoorMovementTransitionFloatCurve, DoorMovementFunctionFloat);
		CurrentLeftRotYaw = LeftDoorMesh->GetComponentRotation().Yaw;
		CurrentRightRotYaw = RightDoorMesh->GetComponentRotation().Yaw;
		UE_LOG(LogTemp, Warning, TEXT("Setting Door Movement Transition"));
	}
}

void ACabinet::InteractWithItem(AMainCharacter* InCharacter) {
	FString CurrentHitItemName = Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Interact With Component ::> %s"), *CurrentHitItemName)

	// if (Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent() == LeftDoorMesh || Character->
	// GetCharacterItemComponent()->GetTraceHitItemHitComponent() == RightDoorMesh) {

	bIsOpenedRef = !bIsOpened;
	DoorMovementTransitionTimeline->PlayFromStart();
	// }
	// else {
	// Super::InteractWithItem(InCharacter);
	// }
}

void ACabinet::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bIsOpenedRef != bIsOpened) {
		UE_LOG(LogTemp, Warning, TEXT("Updating Door Rot: %f"), CurrentDoorRotation);
		FRotator LRot = LeftDoorMesh->GetComponentRotation();
		FRotator RRot = RightDoorMesh->GetComponentRotation();

		float LeftLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotYaw
			                                                : CurrentLeftRotYaw - 110,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotYaw - 110
			                                                : CurrentLeftRotYaw,
		                                                CurrentDoorRotation);
		float RightLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentLeftRotYaw
			                                                 : CurrentLeftRotYaw + 110,
		                                                 bIsOpenedRef
			                                                 ? CurrentLeftRotYaw + 110
			                                                 : CurrentLeftRotYaw,
		                                                 CurrentDoorRotation);

		// float LeftLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef ? 110 : 0,
		//                                                 bIsOpenedRef ? 0 : 110,
		//                                                 CurrentDoorRotation);
		// float RightLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef ? -110 : 0,
		//                                                  bIsOpenedRef ? 0 : -110,
		//                                                  CurrentDoorRotation);

		FRotator LNewRot = {LRot.Pitch, LeftLerpRotYaw, LRot.Roll};
		FRotator RNewRot = {LRot.Pitch, RightLerpRotYaw, LRot.Roll};

		LeftDoorMesh->SetWorldLocationAndRotation(LeftDoorMesh->GetComponentLocation(), LNewRot);
		RightDoorMesh->SetWorldLocationAndRotation(RightDoorMesh->GetComponentLocation(), RNewRot);
		if (CurrentDoorRotation == 1.f) {
			bIsOpened = !bIsOpened;
			CurrentDoorRotation = 0;
		}
	}
}

void ACabinet::UpdateDoorMovementTransitionTimeline(float Output) {
	UE_LOG(LogTemp, Error, TEXT("Updating Door Rot: %f"), CurrentDoorRotation);
	CurrentDoorRotation = Output;
}
