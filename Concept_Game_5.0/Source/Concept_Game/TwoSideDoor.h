// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "TwoSideDoor.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ATwoSideDoor : public ADoor {
	GENERATED_BODY()

public:
	ATwoSideDoor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetDoorRotation(FRotator LeftRotator, FRotator RightRotator);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationYawLeftAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationYawRightAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationPitchLeftAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationPitchRightAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationRollLeftAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float RotationRollRightAngle;

	float CurrentLeftRotYaw;

	float CurrentRightRotYaw;
};
