// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "UObject/Interface.h"
#include "TeamInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeamInfoInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class CONCEPT_GAME_API ITeamInfoInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetTeamID();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsTargetAnEnemy(int32 TeamID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsAlive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PickupWeapon(AWeapon* InWeapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropItem(AItem* ItemToDrop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchWeapon(AWeapon* InWeapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UseCurrentItem();
};
