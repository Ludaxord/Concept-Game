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

void AFireWeapon::InitializeWeaponDataTable() {
	Super::InitializeWeaponDataTable();
	const FString
		FireWeaponTablePath(TEXT("DataTable'/Game/_Game/DataTables/FireWeaponDataTable.FireWeaponDataTable'"));
	UDataTable* FireWeaponTableObject = Cast<UDataTable>(
		StaticLoadObject(UDataTable::StaticClass(), nullptr, *FireWeaponTablePath));

	if (FireWeaponTableObject) {
		UE_LOG(LogTemp, Warning, TEXT("Fire Weapon Data Table Loaded"));
		FFireWeaponDataTable* FireWeaponDataRow = nullptr;
		switch (FireWeaponType) {
		case EFireWeaponType::EFWT_Beretta:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Beretta-Pistol"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_AR4:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AR4-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_Glock:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Glock-Pistol"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_Colt:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Colt-Pistol"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_AK47:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AK47-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_AKVal:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AKVal-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_SMG1:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(
				FName("SMG11-SubmachineGun"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_AA12:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AA12-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_UZI:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(
				FName("UZI-SubmachineGun"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_MP40:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("MP40-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_AK74:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AK74-Rifle"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaBeretta:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Beretta-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaAA12:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AA12-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaGlock:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Glock-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaColt:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("Colt-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaAK47:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("AK47-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaUZI:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("UZI-Futura"),TEXT(""));
			break;
		case EFireWeaponType::EFWT_FuturaMP40:
			FireWeaponDataRow = FireWeaponTableObject->FindRow<FFireWeaponDataTable>(FName("MP40-Futura"),TEXT(""));
			break;
		}

		if (FireWeaponDataRow) {
			UE_LOG(LogTemp, Warning, TEXT("Fire Weapon Data Row Loaded: %s"), *FireWeaponDataRow->ItemName);

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to load Fire Weapon Data Row"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load Fire Weapon Data Table"));
	}
}

void AFireWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AFireWeapon::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	InitializeWeaponDataTable();
}
