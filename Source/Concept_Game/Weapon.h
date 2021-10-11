// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponType.h"
#include "Engine/DataTable.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AWeapon : public AItem {
	GENERATED_BODY()
public:
	AWeapon();
protected:
	FTimerHandle ThrowWeaponTimer;

	float ThrowWeaponTime;

	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta =(AllowPrivateAccess = "true"))
	FName BoneToHide;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "DataTable", meta = (AllowPrivateAccess = "true"))
	class USoundCue* UseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;
private:
};
