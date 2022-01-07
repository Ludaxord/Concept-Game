// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItem.h"

APhysicsBasedItem::APhysicsBasedItem() {
}

bool APhysicsBasedItem::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return IPhysicsBasedInteractionInterface::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsBasedItem::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	APlayerController* InController, FText& OutMessage, UPrimitiveComponent* OutLookAtComponent) {
	return IPhysicsBasedInteractionInterface::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage,
	                                                                OutLookAtComponent);
}
