// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableWeaponType.h"
#include "Weapon.h"
#include "ThrowableWeapon.generated.h"


USTRUCT()
struct FThrowableWeaponDataTable : public FWeaponDataTable {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EThrowableWeaponType ThrowableWeaponType;
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AThrowableWeapon : public AWeapon {
	GENERATED_BODY()
public:
	AThrowableWeapon();

	virtual bool GetUsability() override;

	virtual void PerformAttack(ACharacter* RefCharacter) override;

	virtual void DecreaseUsability() override;

	virtual void StartWeaponAnimationTimer() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EThrowableWeaponType ThrowableWeaponType;
};
