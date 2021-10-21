// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon() {
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

bool AWeapon::GetUsability() {
	return false;
}

void AWeapon::PerformAttack() {
}

void AWeapon::DecreaseUsability() {
}

void AWeapon::StartWeaponAnimationTimer() {
}

void AWeapon::InitializeWeaponDataTable() {
}

bool AWeapon::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult,
                                 AMainCharacter* InCharacter) {
	FVector OutBeamLocation;
	FHitResult CrosshairHitResult;

	if (Character == nullptr) {
		Character = InCharacter;
	}

	if (Character) {
		bool bCrosshairHit = Character->TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);
		if (bCrosshairHit) {
			OutBeamLocation = CrosshairHitResult.Location;
		}
		const FVector WeaponTraceStart = MuzzleSocketLocation;
		const FVector WeaponTraceEnd = OutBeamLocation;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, WeaponTraceStart, WeaponTraceEnd,
		                                     ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
			UE_LOG(LogTemp, Warning, TEXT("Trace line position: %s HIT: %s"), *OutHitResult.Location.ToString(),
		       *OutHitResult.Actor->GetName())


		if (!OutHitResult.bBlockingHit) {
			OutHitResult.Location = OutBeamLocation;
			return false;
		}
	}


	return true;

}

void AWeapon::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
}

void AWeapon::PerformTrace(AMainCharacter* InMainCharacter) {
	Super::PerformTrace(InMainCharacter);
}

void AWeapon::LeaveTrace(AMainCharacter* InMainCharacter) {
	Super::LeaveTrace(InMainCharacter);
}

void AWeapon::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	if (BoneToHide != FName("")) {
		GetItemMesh()->HideBoneByName(BoneToHide, PBO_None);
	}
}

void AWeapon::StopFalling() {
}
