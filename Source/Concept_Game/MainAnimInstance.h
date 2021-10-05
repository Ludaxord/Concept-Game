// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AttackType.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UMainAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	UMainAnimInstance();

	FORCEINLINE EAttackType GetCurrentAttackType() const {
		return CurrentAttackType;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat", meta= (AllowPrivateAccess = "true"))
	EAttackType CurrentAttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta= (AllowPrivateAccess = "true"))
	bool bIsAimingAvailable;
};
