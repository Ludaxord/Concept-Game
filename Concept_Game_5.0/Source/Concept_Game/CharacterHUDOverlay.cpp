// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUDOverlay.h"

#include "InventoryComponent.h"
#include "MainCharacter.h"

bool UCharacterHUDOverlay::ToggleVisibilityOfQuickSelect() {
	CastCharacterComponents();

	if (CharacterInventoryComponent) {
		return CharacterInventoryComponent->GetQuickSelectVisibility();
	}

	return false;
}

bool UCharacterHUDOverlay::ToggleVisibilityOfPlayerHUD() {
	CastCharacterComponents();
	bool bVisibility = true;
	if (Character) {
		bVisibility = Character->GetPlayerHUDVisibility();
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerHUDVisibility: %s"), bVisibility ? TEXT("true") : TEXT("false"));

	return bVisibility;
}

void UCharacterHUDOverlay::CastCharacterComponents() {
	if (!Character) {
		Character = Cast<AMainCharacter>(GetOwningPlayerPawn());
	}

	if (!CharacterInventoryComponent) {
		CharacterInventoryComponent = Character->GetCharacterInventoryComponent();
	}
}
