// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Cabinet.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ACabinet : public APhysicsBasedItem {
	GENERATED_BODY()
public:
	ACabinet();

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* LeftDoorPhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* RightDoorPhysicsConstraint;
};
