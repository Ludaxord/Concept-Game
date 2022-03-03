// Fill out your copyright notice in the Description page of Project Settings.


#include "CamoSkill.h"

#include "MainCharacter.h"
#include "RenderComponent.h"

void UCamoSkill::ApplySkill(AActor* InOwner) {
	Super::ApplySkill(InOwner);
	AMainCharacter* Character = Cast<AMainCharacter>(InOwner);
	if (Character) {
		if (Character->GetCharacterRenderingComponent()) {
			Character->GetCharacterRenderingComponent()->ApplyInvisibility(true);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("CharacterRenderingComponent is NULLPTR"))
		}
	}
}

void UCamoSkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
                      UTexture2D* InImage) {
	Super::Init(InCategory, InName, InDescription, InPointsAmount, InImage);
}
