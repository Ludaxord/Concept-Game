// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsInteractionItem.h"
#include "PhysicsBasedTestObject.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API APhysicsBasedTestObject : public APhysicsInteractionItem {
	GENERATED_BODY()
public:
	APhysicsBasedTestObject();

	virtual bool InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) override;

	virtual AActor* LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	                                      APlayerController* InController, FText& OutMessage,
	                                      UPrimitiveComponent* OutLookAtComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TestMesh;
};
