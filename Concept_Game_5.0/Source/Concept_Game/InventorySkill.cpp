// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySkill.h"

void UInventorySkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);
}

void UInventorySkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
