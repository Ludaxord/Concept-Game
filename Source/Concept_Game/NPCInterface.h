// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackType.h"
#include "MainCharacter.h"
#include "PoseType.h"
#include "CombatState.h"
#include "UObject/Interface.h"
#include "NPCInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class CONCEPT_GAME_API INPCInterface {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(class AMainCharacter* InCharacter);

	UFUNCTION(BlueprintNativeEvent)
	bool GetAiming();

	UFUNCTION(BlueprintNativeEvent)
	bool IsAbleToAttack();

	UFUNCTION(BlueprintNativeEvent)
	EAttackType GetCurrentAttackType();

	UFUNCTION(BlueprintNativeEvent)
	EPoseType GetCurrentPoseType();

	UFUNCTION(BlueprintNativeEvent)
	ECombatState GetCombatState();

	UFUNCTION(BlueprintNativeEvent)
	class AItem* GetEquippedItem();

	UFUNCTION(BlueprintNativeEvent)
	void EquipItem(AItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	bool GetInCover();

	UFUNCTION(BlueprintNativeEvent)
	bool GetCoveringActive();

	UFUNCTION(BlueprintNativeEvent)
	bool GetMoveInCover();

	UFUNCTION(BlueprintNativeEvent)
	bool GetRunning();
};
