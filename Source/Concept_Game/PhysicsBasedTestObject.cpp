// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedTestObject.h"

APhysicsBasedTestObject::APhysicsBasedTestObject() {
	TestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	SetRootComponent(TestMesh);

	TestMesh->SetSimulatePhysics(true);
	TestMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
}

bool APhysicsBasedTestObject::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return Super::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsBasedTestObject::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
                                                       APlayerController* InController, FText& OutMessage,
                                                       UPrimitiveComponent* OutLookAtComponent) {
	return Super::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage, OutLookAtComponent);
}
