// Fill out your copyright notice in the Description page of Project Settings.


#include "CyberWeapon.h"

ACyberWeapon::ACyberWeapon() {
}

//TODO: Add Manna and check if player has enough Manna..
bool ACyberWeapon::GetUsability() {
	return false;
}

void ACyberWeapon::PerformAttack() {
	
}

void ACyberWeapon::DecreaseUsability() {
	
}

void ACyberWeapon::StartWeaponAnimationTimer() {
	
}

int32 ACyberWeapon::GetWeaponUsability() {
	return Super::GetWeaponUsability();
}
