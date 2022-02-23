// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPPanicTaskComponent.h"

#include "NPCBase.h"
#include "WorldStateManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UGOAPPanicTaskComponent::UGOAPPanicTaskComponent() {
	TaskName = "Panic"; 
}

void UGOAPPanicTaskComponent::BeginPlay() {
	Super::BeginPlay();
}

void UGOAPPanicTaskComponent::CallActors() {
	Super::CallActors();
}

bool UGOAPPanicTaskComponent::PrePerform() {
	UE_LOG(LogTemp, Warning, TEXT("=================== UGOAPPanicTaskComponent::PrePerform"))
	if (ANPCBase* NPCOwner = Cast<ANPCBase>(GetOwner())) {
		NPCOwner->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}

	return true;
}

bool UGOAPPanicTaskComponent::PostPerform() {
	UE_LOG(LogTemp, Warning, TEXT("=================== UGOAPPanicTaskComponent::PostPerform"))

	AWorldStateManager* StateManager = Cast<AWorldStateManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AWorldStateManager::StaticClass()));
	StateManager->RemoveState("Fire Weapon");

	return true;
}

bool UGOAPPanicTaskComponent::IsViable() {
	return Super::IsViable();
}
