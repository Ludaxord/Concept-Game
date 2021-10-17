// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"

AFireWeapon::AFireWeapon() {
}

bool AFireWeapon::GetUsability() {
	return GetAmmo() > 0 && GetItemRarity() != EItemRarity::EIR_Damaged;
}

void AFireWeapon::PerformAttack(ACharacter* RefCharacter) {

}

void AFireWeapon::DecreaseUsability() {
}

void AFireWeapon::StartWeaponAnimationTimer() {
}

void AFireWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AFireWeapon::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}
