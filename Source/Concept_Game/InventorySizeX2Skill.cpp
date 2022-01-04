// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySizeX2Skill.h"

void UInventorySizeX2Skill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);
}

void UInventorySizeX2Skill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
