// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreasedSpeedSkill.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UIncreasedSpeedSkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);

	AMainCharacter* Character = Cast<AMainCharacter>(InOwner);
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}

}

void UIncreasedSpeedSkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
                                UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
