// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogComponent.h"

// Sets default values for this component's properties
UNPCDialogComponent::UNPCDialogComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UNPCDialogComponent::BeginPlay() {
	Super::BeginPlay();
}


// Called every frame
void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
