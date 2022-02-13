// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPFreeRoamTaskComponent.h"

#include "FreeRoamPoint.h"
#include "NPCBase.h"
#include "WorldStateManager.h"

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
		UE_LOG(LogTemp, Warning, TEXT("UGOAPFreeRoamTaskComponent::PostPerform => bLoopTaskTillBreak"))
		TaskOwner->GetStateManager()->RemoveState(FString("CanWalk"));
		return true;
	}

	return Super::PostPerform();
}

bool UGOAPFreeRoamTaskComponent::IsViable() {
	return Super::IsViable();
}
