// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPFreeRoamTaskComponent.h"

#include "FreeRoamPoint.h"

UGOAPFreeRoamTaskComponent::UGOAPFreeRoamTaskComponent() {
	TaskName = "Free Roam";
}

void UGOAPFreeRoamTaskComponent::BeginPlay() {
	Super::BeginPlay();
}

void UGOAPFreeRoamTaskComponent::CallActors() {
	Super::CallActors();
	AttachActors(AFreeRoamPoint::StaticClass());
}

bool UGOAPFreeRoamTaskComponent::PrePerform() {
	return Super::PrePerform();
}

bool UGOAPFreeRoamTaskComponent::PostPerform() {
	if (!bLoopTaskTillBreak) {
		return true;
	}

	return Super::PostPerform();
}

bool UGOAPFreeRoamTaskComponent::IsViable() {
	return Super::IsViable();
}
