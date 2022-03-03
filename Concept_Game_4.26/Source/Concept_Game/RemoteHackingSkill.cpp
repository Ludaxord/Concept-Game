// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteHackingSkill.h"

void URemoteHackingSkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);
}

void URemoteHackingSkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
