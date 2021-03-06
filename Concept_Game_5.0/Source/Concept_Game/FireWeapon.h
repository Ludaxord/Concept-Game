// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "FireWeaponType.h"
#include "MagazineType.h"
#include "Weapon.h"
#include "FireWeapon.generated.h"

USTRUCT()
struct FFireWeaponDataTable : public FWeaponDataTable {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireWeaponType FireWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireWeaponAnimType FireWeaponAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagazineType MagazineType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AFireWeapon : public AWeapon {
	GENERATED_BODY()
public:
	AFireWeapon();

	virtual bool GetUsability() override;

	virtual void PerformAttack() override;

	virtual void DecreaseUsability() override;

	virtual void StartWeaponAnimationTimer() override;

	virtual void InitializeWeaponDataTable() override;

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual int32 GetWeaponUsability() override;

	int32 GetAmmo() const {
		return Ammo;
	}


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EFireWeaponType FireWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EFireWeaponAnimType FireWeaponAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType MagazineType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float HeadShotDamage;
private:
	int32 PreviousMaterialIndex;

public:
	void SetReloadMontageSection(const FName& Name) {
		ReloadMontageSection = Name;
	}

	UParticleSystem* GetMuzzleFlash() const {
		return MuzzleFlash;
	}
};
