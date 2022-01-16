// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Drawer.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ADrawer : public APhysicsBasedItem
{
	GENERATED_BODY()

public:
	ADrawer();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TopDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BottomDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MiddleDrawerMesh;
};
