// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPDialogTaskComponent.h"

#include "NPCBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	UE_LOG(LogTemp, Warning, TEXT("00000000000000 UGOAPDialogTaskComponent::PrePerform 00000000000000"))
	if (ANPCBase* NPC = Cast<ANPCBase>(GetOwner())) {
		NPC->GetController()->StopMovement();
		UE_LOG(LogTemp, Warning, TEXT("UGOAPDialogTaskComponent::PrePerform -> %s"), *NPC->GetName())
		return Super::PrePerform();
	}

	return false;
}

bool UGOAPDialogTaskComponent::PostPerform() {
	return Super::PostPerform();
}

bool UGOAPDialogTaskComponent::IsViable() {
	return Super::IsViable();
}

bool UGOAPDialogTaskComponent::StartAction() {
	return true;
}

bool UGOAPDialogTaskComponent::PerformAction() {
	//TODO: Check if Player pick option from menu....
	// return Super::PerformAction();
	if (ANPCBase* NPC = Cast<ANPCBase>(GetOwner())) {
		return !NPC->GetQuestWidgetActive();
	}

	return false;
}
