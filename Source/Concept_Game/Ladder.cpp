// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALadder::ALadder(): RungsNumber(10),
                    SpaceBetweenRungs(40.0f),
                    LadderMeshName("'/Game/_Game/Assets/Meshes/Ladder/ladder_part_pivot_center_static_mesh'") {
	LadderMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("LadderMesh"));

	RootLadderMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
		FName("RootLadderMeshComponent"));
	SetRootComponent(RootLadderMeshComponent);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());

	LadderCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LadderCollisionSphere"));
	LadderCollisionSphere->SetupAttachment(GetRootComponent());

	// SetupLadderRungs();
}

void ALadder::SetupLadderRungs() {
	for (int i = 0; i <= RungsNumber - 1; i++) {
		FString FStaticMeshComponentName = "LadderMeshComponent_" + FString::FromInt(i);
		UStaticMeshComponent* LadderMeshComponent = NewObject<UStaticMeshComponent>(this, *FStaticMeshComponentName);
		FVector MeshLocation = {
			GetRootComponent()->GetComponentLocation().X,
			GetRootComponent()->GetComponentLocation().Y,
			GetRootComponent()->GetComponentLocation().Z + (i * SpaceBetweenRungs)
		};
		FRotator MeshRotation = {
			GetItemMesh()->GetComponentRotation().Pitch,
			GetItemMesh()->GetComponentRotation().Yaw,
			GetItemMesh()->GetComponentRotation().Roll
		};

		FTransform LadderTransform = FTransform(MeshRotation, MeshLocation);

		// LadderMeshComponent->SetWorldLocation(MeshLocation, false, nullptr, ETeleportType::TeleportPhysics);
		// LadderMeshComponent->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
		LadderMeshComponent->SetWorldTransform(LadderTransform, false, nullptr, ETeleportType::TeleportPhysics);

		// UStaticMeshComponent* LadderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStaticMeshComponentName);
		// ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(*LadderMeshName);
		// LadderMeshComponent->SetStaticMesh(Mesh.Object);

		LadderMeshComponent->SetStaticMesh(LadderMesh);
		LadderMeshComponent->RegisterComponent();
		LadderMeshComponent->OnComponentCreated();

		LadderMeshComponent->SetRelativeTransform(LadderTransform);
		LadderMeshComponent->SetupAttachment(GetRootComponent());
	}
}
