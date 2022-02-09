// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPPanicTaskComponent.h"

#include "NPCBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGOAPPanicTaskComponent::BeginPlay() {
	Super::BeginPlay();
}

void UGOAPPanicTaskComponent::CallActors() {
	Super::CallActors();
}

bool UGOAPPanicTaskComponent::PrePerform() {
	if (ANPCBase* NPCOwner = Cast<ANPCBase>(GetOwner())) { 
		NPCOwner->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}

	return Super::PrePerform();
}

bool UGOAPPanicTaskComponent::PostPerform() {
	return Super::PostPerform();
}

bool UGOAPPanicTaskComponent::IsViable() {
	return Super::IsViable();
}
