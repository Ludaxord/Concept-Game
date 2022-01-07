// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsInteractionItem.h"

// Sets default values
APhysicsInteractionItem::APhysicsInteractionItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APhysicsInteractionItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsInteractionItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APhysicsInteractionItem::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return IPhysicsBasedInteractionInterface::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsInteractionItem::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	APlayerController* InController, FText& OutMessage, UPrimitiveComponent* OutLookAtComponent) {
	return IPhysicsBasedInteractionInterface::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage,
	                                                                OutLookAtComponent);
}


