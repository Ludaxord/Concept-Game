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

	float CurrentLeftRotYaw;

	float CurrentRightRotYaw;
};
