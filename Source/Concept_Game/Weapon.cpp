// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
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

bool AWeapon::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult) {
	FVector OutBeamLocation;
	FHitResult CrosshairHitResult;
	if (TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation)) {
		OutBeamLocation = CrosshairHitResult.Location;
	}
	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector WeaponTraceEnd = OutBeamLocation;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, WeaponTraceStart, WeaponTraceEnd,
	                                     ECollisionChannel::ECC_Visibility);
	if (!OutHitResult.bBlockingHit) {
		OutHitResult.Location = OutBeamLocation;
		return false;
	}

	return true;

}

bool AWeapon::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation) {
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation = {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
	CrosshairLocation.Y -= 50.0f;

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld) {
		const FVector Start = CrosshairWorldPosition;
		const FVector End = Start + CrosshairWorldDirection * 50000.0f;
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);

		if (OutHitResult.bBlockingHit) {
			// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
			DrawDebugPoint(GetWorld(), OutHitResult.Location, 100.0f, FColor::Cyan, false, 2.0f);
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void AWeapon::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AWeapon::StopFalling() {
}
