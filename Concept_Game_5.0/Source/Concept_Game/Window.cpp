// Fill out your copyright notice in the Description page of Project Settings.


#include "Window.h"

#include "Kismet/KismetMathLibrary.h"

AWindow::AWindow(): bIsOpened(false) {
	ItemInteractionName = "Open";
	ItemName = "Window";

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	WindowMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(
		TEXT("WindowMovementTransitionTimeline"));
}

void AWindow::BeginPlay() {
	Super::BeginPlay();

	WindowMovementFunctionFloat.BindDynamic(this, &AWindow::UpdateWindowMovementTransitionTimeline);
	if (WindowMovementTransitionFloatCurve) {
		WindowMovementTransitionTimeline->AddInterpFloat(WindowMovementTransitionFloatCurve,
		                                                 WindowMovementFunctionFloat);

		CurrentRotYaw = PhysicsBasedMesh->GetComponentRotation().Yaw;
		CurrentRotPitch = PhysicsBasedMesh->GetComponentRotation().Pitch;
		CurrentRotRoll = PhysicsBasedMesh->GetComponentRotation().Roll;

		CurrentLocX = PhysicsBasedMesh->GetComponentLocation().X;
		CurrentLocY = PhysicsBasedMesh->GetComponentLocation().Y;
		CurrentLocZ = PhysicsBasedMesh->GetComponentLocation().Z;

		UE_LOG(LogTemp, Warning, TEXT("Setting Window Movement Transition"));
	}
}

void AWindow::Tick(float DeltaSeconds) {
	if (bIsOpenedRef != bIsOpened) {
		float LerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                            ? CurrentRotYaw
			                                            : CurrentRotYaw - RotationYawAngle,
		                                            bIsOpenedRef
			                                            ? CurrentRotYaw - RotationYawAngle
			                                            : CurrentRotYaw,
		                                            CurrentWindowRotation);

		float LerpRotPitch = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                              ? CurrentRotPitch
			                                              : CurrentRotPitch - RotationPitchAngle,
		                                              bIsOpenedRef
			                                              ? CurrentRotPitch - RotationPitchAngle
			                                              : CurrentRotPitch,
		                                              CurrentWindowRotation);

		float LerpRotRoll = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                             ? CurrentRotRoll
			                                             : CurrentRotRoll - RotationRollAngle,
		                                             bIsOpenedRef
			                                             ? CurrentRotRoll - RotationRollAngle
			                                             : CurrentRotRoll,
		                                             CurrentWindowRotation);

		float LerpLocX = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                          ? CurrentLocX
			                                          : CurrentLocX - LocationXMovement,
		                                          bIsOpenedRef
			                                          ? CurrentLocX - LocationXMovement
			                                          : CurrentLocX,
		                                          CurrentWindowRotation);

		float LerpLocY = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                          ? CurrentLocY
			                                          : CurrentLocY - LocationYMovement,
		                                          bIsOpenedRef
			                                          ? CurrentLocY - LocationYMovement
			                                          : CurrentLocY,
		                                          CurrentWindowRotation);

		float LerpLocZ = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                          ? CurrentLocZ
			                                          : CurrentLocZ - LocationZMovement,
		                                          bIsOpenedRef
			                                          ? CurrentLocZ - LocationZMovement
			                                          : CurrentLocZ,
		                                          CurrentWindowRotation);

		FRotator NewRot = {LerpRotPitch, LerpRotYaw, LerpRotRoll};
		FVector NewLoc = {LerpLocX, LerpLocY, LerpLocZ};

		SetWindowRotation(NewRot, NewLoc);
	}
}

void AWindow::InteractWithItem(AMainCharacter* InCharacter) {
	bIsOpenedRef = !bIsOpened;
	WindowMovementTransitionTimeline->PlayFromStart();
}

void AWindow::SetWindowRotation(FRotator NewRotator, FVector NewLocation) {
	PhysicsBasedMesh->SetWorldLocationAndRotation(NewLocation, NewRotator);
	if (CurrentWindowRotation == 1.f) {
		bIsOpened = !bIsOpened;
		ItemInteractionName = bIsOpened ? "Close" : "Open";
		CurrentWindowRotation = 0;
	}
}

void AWindow::UpdateWindowMovementTransitionTimeline(float Output) {
	UE_LOG(LogTemp, Warning, TEXT("Update Window Rot... %f"), Output);
	CurrentWindowRotation = Output;
}
