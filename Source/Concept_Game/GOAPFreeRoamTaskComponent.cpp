// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPFreeRoamTaskComponent.h"

#include "FreeRoamPoint.h"

void UGOAPFreeRoamTaskComponent::BeginPlay() {
	Super::BeginPlay();
	AttachActors(AFreeRoamPoint::StaticClass());
}

void UGOAPFreeRoamTaskComponent::CallActors() {
	AttachActors(AFreeRoamPoint::StaticClass());
}

bool UGOAPFreeRoamTaskComponent::PrePerform() {
	UE_LOG(LogTemp, Warning, TEXT("GOAP Pre Perform Free Roam"))
	return Super::PrePerform();
}

bool UGOAPFreeRoamTaskComponent::PostPerform() {
	UE_LOG(LogTemp, Error, TEXT("GOAP Post Perform Free Roam"))
	return Super::PostPerform();
}

bool UGOAPFreeRoamTaskComponent::IsViable() {
	UE_LOG(LogTemp, Error, TEXT("GOAP Is Viable Free Roam"))
	return Super::IsViable();
}
