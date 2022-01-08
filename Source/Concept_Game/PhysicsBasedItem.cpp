// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItem.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

APhysicsBasedItem::APhysicsBasedItem() {
	PhysicsBasedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsBasedMesh"));
	SetRootComponent(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(true);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	CollisionBox->SetupAttachment(PhysicsBasedMesh);

	PickupWidget->SetupAttachment(PhysicsBasedMesh);
	AreaSphere->SetupAttachment(PhysicsBasedMesh);
}

bool APhysicsBasedItem::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return IPhysicsBasedInteractionInterface::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsBasedItem::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
                                                 APlayerController* InController, FText& OutMessage,
                                                 UPrimitiveComponent* OutLookAtComponent) {
	return IPhysicsBasedInteractionInterface::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage,
	                                                                OutLookAtComponent);
}
