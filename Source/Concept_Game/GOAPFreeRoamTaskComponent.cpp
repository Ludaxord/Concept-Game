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
	TaskOwner->GetStateManager()->RemoveState(FString("CanWalk"));

	if (!bLoopTaskTillBreak) {
		UE_LOG(LogTemp, Warning, TEXT("OWNER: %s UGOAPFreeRoamTaskComponent::PostPerform => bLoopTaskTillBreak"),
		       *GetOwner()->GetName())
		return true;
	}

	TaskOwner->GetStateManager()->AddState(FString("CanWalk"), 1);

	return Super::PostPerform();
}

bool UGOAPFreeRoamTaskComponent::IsViable() {
	return Super::IsViable();
}
