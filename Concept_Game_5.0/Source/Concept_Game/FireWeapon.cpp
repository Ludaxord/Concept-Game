// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"

AFireWeapon::AFireWeapon() {
}

bool AFireWeapon::GetUsability() {
	return GetAmmo() > 0 && GetItemRarity() != EItemRarity::EIR_Damaged;
}

void AFireWeapon::PerformAttack() {
	const USkeletalMeshSocket* BarrelSocket = GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket) {
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetItemMesh());
		if (GetMuzzleFlash()) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetMuzzleFlash(), SocketTransform);
		}

		FHitResult BeamHitResult;	
		//TODO: Add Types of actor that is able to be hit by BeamHitResult...
		if (GetBeamEndLocation(SocketTransform.GetLocation(), BeamHitResult)) {
			// if (BeamHitResult.GetActor().IsValid()) {
			if (BeamHitResult.GetActor()) {
				//TODO Add bullet hit interface
				//TODO Add Damage to enemies && objects.
			}
			else {
				if (ImpactParticles) {
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamHitResult.Location);
				}
			}

			if (BeamParticles) {
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), BeamParticles, SocketTransform);
				if (Beam) {
					//NOTE: name "Target" is a part of beam particles file, it should be dynamic to different particle systems.
					Beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
				}
			}
		}
	}
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
			AmmoType = FireWeaponDataRow->AmmoType;
			FireWeaponAnimType = FireWeaponDataRow->FireWeaponAnimType;
			SetWeaponType(FireWeaponDataRow->WeaponType);
			Ammo = FireWeaponDataRow->WeaponAmmo;
			MagazineCapacity = FireWeaponDataRow->MagazineCapacity;
			SetPickupSound(FireWeaponDataRow->PickupSound);
			SetEquipSound(FireWeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(FireWeaponDataRow->ItemMesh);
			SetItemName(FireWeaponDataRow->ItemName);

			SetMaterialInstance(FireWeaponDataRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(FireWeaponDataRow->MaterialIndex);
			SetClipBoneName(FireWeaponDataRow->ClipBoneName);
			SetReloadMontageSection(FireWeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(FireWeaponDataRow->AnimBP);
			CrosshairsMiddle = FireWeaponDataRow->CrosshairsMiddle;
			CrosshairsTop = FireWeaponDataRow->CrosshairsTop;
			CrosshairsBottom = FireWeaponDataRow->CrosshairsBottom;
			CrosshairsLeft = FireWeaponDataRow->CrosshairsLeft;
			CrosshairsRight = FireWeaponDataRow->CrosshairsRight;
			AutoFireRate = FireWeaponDataRow->AutoFireRate;
			MuzzleFlash = FireWeaponDataRow->MuzzleFlash;
			ImpactParticles = FireWeaponDataRow->ImpactParticles;
			BeamParticles = FireWeaponDataRow->BeamParticles;
			UseSound = FireWeaponDataRow->UseSound;
			BoneToHide = FireWeaponDataRow->BoneToHide;
			bAutomatic = FireWeaponDataRow->bAutomatic;
			Damage = FireWeaponDataRow->BaseDamage;
			HeadShotDamage = FireWeaponDataRow->BaseHeadShotDamage;
			bThrowable = FireWeaponDataRow->bThrowable;
			SetItemDimensions(FireWeaponDataRow->ItemDimensions);
			SetInventoryItemIconRotated(FireWeaponDataRow->InventoryItemIcon);
			SetInventoryItemIcon(FireWeaponDataRow->InventoryItemIconRotated);
			ContextMenus = FireWeaponDataRow->ContextMenus;
			bAllowQuickSelect = FireWeaponDataRow->bAllowQuickSelect;
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
