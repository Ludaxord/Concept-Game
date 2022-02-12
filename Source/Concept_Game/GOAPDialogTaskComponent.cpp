// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPDialogTaskComponent.h"

#include "GameFramework/Character.h"

UGOAPDialogTaskComponent::UGOAPDialogTaskComponent() {
	TaskName = "Dialog";
}

void UGOAPDialogTaskComponent::BeginPlay() {
	Super::BeginPlay();
	AttachActors(ACharacter::StaticClass());
}

void UGOAPDialogTaskComponent::CallActors() {
	Super::CallActors();
	AttachActors(ACharacter::StaticClass());
}

bool UGOAPDialogTaskComponent::PrePerform() {
	return Super::PrePerform();
}

bool UGOAPDialogTaskComponent::PostPerform() {
	return Super::PostPerform();
}

bool UGOAPDialogTaskComponent::IsViable() {
	return Super::IsViable();
}
