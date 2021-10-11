// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CyberWeaponType.h"
#include "Weapon.h"
#include "CyberWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ACyberWeapon : public AWeapon {
	GENERATED_BODY()

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	ECyberWeaponType CyberWeaponType;
private:
};
