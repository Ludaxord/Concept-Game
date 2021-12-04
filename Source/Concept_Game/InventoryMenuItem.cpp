// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenuItem.h"

#include "MainCharacter.h"

FText UInventoryMenuItem::GetAmmo() {
	CastCharacterComponents();

	if (CharacterInventoryComponent) {

	}

	return FText::FromString("0/0");
}

FText UInventoryMenuItem::GetWeaponName() {
	return FText::FromString("Undefined");
}

FText UInventoryMenuItem::GetWeaponType() {
	return FText::FromString("Undefined");
}

float UInventoryMenuItem::GetWeaponDamagePercent() {
	return 0.f;
}

void UInventoryMenuItem::CastCharacterComponents() {
	if (!Character) {
		Character = Cast<AMainCharacter>(GetOwningPlayerPawn());
	}

	if (!CharacterInventoryComponent) {
		CharacterInventoryComponent = Character->GetCharacterInventoryComponent();
	}
}
