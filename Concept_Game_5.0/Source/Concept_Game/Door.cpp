// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ADoor::ADoor(): bIsOpened(false), bIsOpenedRef(false), RotationYawAngle(110.f) {
	ItemInteractionName = "Open";
	ItemName = "Door";

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	DoorMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorMovementTransitionTimeline"));
}

void ADoor::BeginPlay() {
	Super::BeginPlay();

	DoorMovementFunctionFloat.BindDynamic(this, &ADoor::UpdateDoorMovementTransitionTimeline);
	if (DoorMovementTransitionFloatCurve) {
		DoorMovementTransitionTimeline->AddInterpFloat(DoorMovementTransitionFloatCurve, DoorMovementFunctionFloat);
		CurrentRotYaw = PhysicsBasedMesh->GetComponentRotation().Yaw;
		UE_LOG(LogTemp, Warning, TEXT("Setting Door Movement Transition"));
	}
}

void ADoor::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (bIsOpenedRef != bIsOpened) {
		FRotator Rot = PhysicsBasedMesh->GetComponentRotation();

		float LerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                            ? CurrentRotYaw
			                                            : CurrentRotYaw - RotationYawAngle,
		                                            bIsOpenedRef
			                                            ? CurrentRotYaw - RotationYawAngle
			                                            : CurrentRotYaw,
		                                            CurrentDoorRotation);

		FRotator NewRot = {Rot.Pitch, LerpRotYaw, Rot.Roll};

		PhysicsBasedMesh->SetWorldLocationAndRotation(PhysicsBasedMesh->GetComponentLocation(), NewRot);

		if (CurrentDoorRotation == 1.f) {
			bIsOpened = !bIsOpened;
			ItemInteractionName = bIsOpened ? "Close" : "Open";
			CurrentDoorRotation = 0;
		}
	}
}

void ADoor::InteractWithItem(AMainCharacter* InCharacter) {
	bIsOpenedRef = !bIsOpened;
	DoorMovementTransitionTimeline->PlayFromStart();
}

void ADoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult) {
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnBoxEndOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
}

void ADoor::UpdateDoorMovementTransitionTimeline(float Output) {
	UE_LOG(LogTemp, Warning, TEXT("Update Door Rot... %f"), Output);
	CurrentDoorRotation = Output;
}
