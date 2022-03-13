// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoSideDoor.h"
#include "TwoSideTwoAxisDoor.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ATwoSideTwoAxisDoor : public ATwoSideDoor {
	GENERATED_BODY()
public:
	ATwoSideTwoAxisDoor();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationPitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationRollAngle;

	float CurrentLeftRotPitch;

	float CurrentRightRotPitch;

	float CurrentLeftRotRoll;

	float CurrentRightRotRoll;
};
