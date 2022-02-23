// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemiesMinimapHighlightSkill.h"

void UEnemiesMinimapHighlightSkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);
}

void UEnemiesMinimapHighlightSkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
