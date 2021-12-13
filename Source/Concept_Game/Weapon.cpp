// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
#include "InventoryComponent.h"
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

		// DrawDebugLine(GetWorld(), WeaponTraceStart, WeaponTraceEnd, FColor::Red, false, 50.0f);

		if (!OutHitResult.bBlockingHit) {
			OutHitResult.Location = OutBeamLocation;
			return false;
		}
	}


	return true;

}

void AWeapon::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
	//TODO: create inventory and check if there is place in inventory
	//TODO: If there is a place then swap weapon and currently equipped store in inventory
	//TODO: If there is no place in inventory, show error message.
	if (Character) {
		Character->GetCharacterInventoryComponent()->AddInventoryItem(this);		
	}
	// Character->SwapWeapon(this);
}

void AWeapon::PerformTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids) {
	Super::PerformTrace(InMainCharacter, Guids);
}

void AWeapon::LeaveTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids) {
	Super::LeaveTrace(InMainCharacter, Guids);
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
