// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "PhysicsBasedItemTestObject.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API APhysicsBasedItemTestObject : public APhysicsBasedItem
{
	GENERATED_BODY()
	
public:
	APhysicsBasedItemTestObject();

	virtual bool InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) override;

	virtual AActor* LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	                                      APlayerController* InController, FText& OutMessage,
	                                      UPrimitiveComponent* OutLookAtComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TestMesh;
};
