// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"

ADrawer::ADrawer() {
	ItemInteractionName = "Open";
	ItemName = "Drawer";

	TopDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopDrawerMesh"));
	BottomDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomDrawerMesh"));
	MiddleDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiddleDrawerMesh"));

	
	TopDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	BottomDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	MiddleDrawerMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	TopDrawerMesh->SetSimulatePhysics(true);
	TopDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	BottomDrawerMesh->SetSimulatePhysics(true);
	BottomDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	MiddleDrawerMesh->SetSimulatePhysics(true);
	MiddleDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
}
