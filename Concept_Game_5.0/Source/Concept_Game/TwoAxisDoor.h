// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "TwoAxisDoor.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ATwoAxisDoor : public ADoor {
	GENERATED_BODY()
public:
	ADoor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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
