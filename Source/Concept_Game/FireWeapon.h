// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "FireWeaponType.h"
#include "Weapon.h"
#include "FireWeapon.generated.h"

USTRUCT()
struct FFireWeaponDataTable : public FWeaponDataTable {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireWeaponType FireWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EFireWeaponType FireWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float HeadShotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;
private:
};
