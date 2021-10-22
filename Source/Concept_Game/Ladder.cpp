// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

ALadder::ALadder(): RungsNumber(10) {
		LadderMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("LadderMesh"));

	//TODO: Move to procedural generation in OnConstruction
	LadderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMeshComponent"));
	SetRootComponent(LadderMeshComponent);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());


	LadderCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LadderCollisionSphere"));
	LadderCollisionSphere->SetupAttachment(GetRootComponent());
	LadderCollisionSphere->SetSphereRadius(50.0f);
}

void ALadder::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	//TODO: Create Rungs based on Rungs Number (loop through Rungs).
}
