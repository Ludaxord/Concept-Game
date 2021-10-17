// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponType.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

USTRUCT()
struct FMeleeWeaponDataTable : public FWeaponDataTable {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeleeWeaponType MeleeWeaponType;
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AMeleeWeapon : public AWeapon {
	GENERATED_BODY()
public:
	AMeleeWeapon();

	virtual bool GetUsability() override;

	virtual void PerformAttack(ACharacter* RefCharacter) override;

	virtual void DecreaseUsability() override;

	virtual void StartWeaponAnimationTimer() override;
protected:
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EMeleeWeaponType MeleeWeaponType;
};
