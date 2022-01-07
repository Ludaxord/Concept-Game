// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItemTestObject.h"

APhysicsBasedItemTestObject::APhysicsBasedItemTestObject() {
	TestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	SetRootComponent(TestMesh);

	TestMesh->SetSimulatePhysics(true);
	TestMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
}

bool APhysicsBasedItemTestObject::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return Super::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsBasedItemTestObject::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
                                                       APlayerController* InController, FText& OutMessage,
                                                       UPrimitiveComponent* OutLookAtComponent) {
	return Super::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage, OutLookAtComponent);
}

