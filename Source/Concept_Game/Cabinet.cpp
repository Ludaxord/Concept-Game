// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabinet.h"

ACabinet::ACabinet() {
	ItemInteractionName = "Open";
	ItemName = "Cabinet";

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));

	LeftDoorMesh->SetupAttachment(PhysicsBasedMesh);
	RightDoorMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(true);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftDoorPhysicsConstraint"));
	RightDoorPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightDoorPhysicsConstraint"));

	LeftDoorPhysicsConstraint->SetupAttachment(LeftDoorMesh);
	RightDoorPhysicsConstraint->SetupAttachment(RightDoorMesh);

	LeftDoorPhysicsConstraint->ComponentName1 = {"LeftDoorMesh"};
	LeftDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};

	RightDoorPhysicsConstraint->ComponentName1 = {"RightDoorMesh"};
	RightDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};
}

void ACabinet::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
}
