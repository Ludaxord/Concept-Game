// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleJumpSkill.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDoubleJumpSkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);

	AMainCharacter* Character = Cast<AMainCharacter>(InOwner);
	if (Character) {
		Character->JumpMaxCount = 2;
	}
}

void UDoubleJumpSkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
