// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

AMeleeWeapon::AMeleeWeapon() {
}

bool AMeleeWeapon::GetUsability() {
	return GetItemRarity() != EItemRarity::EIR_Damaged;
}

void AMeleeWeapon::PerformAttack(ACharacter* RefCharacter) {

}

void AMeleeWeapon::DecreaseUsability() {
}

void AMeleeWeapon::StartWeaponAnimationTimer() {
}
