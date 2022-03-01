// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogComponent.h"

#include "NPCBase.h"

UDialogComponent::UDialogComponent() {

}

void UDialogComponent::BeginPlay() {
	Super::BeginPlay();

	if (ANPCBase* NPCOwner = Cast<ANPCBase>(GetOwner())) {
		AAIController* AIController = NPCOwner->GetAIController();
	}
}
