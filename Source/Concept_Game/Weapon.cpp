// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon() {
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AWeapon::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AWeapon::StopFalling() {
}
