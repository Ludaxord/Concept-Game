// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PhysicsBasedInteractionInterface.h"
#include "PhysicsBasedItem.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API APhysicsBasedItem : public AItem, public IPhysicsBasedInteractionInterface {
	GENERATED_BODY()
public:
	APhysicsBasedItem();

	virtual bool InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) override;

	virtual AActor* LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	                                      APlayerController* InController, FText& OutMessage,
	                                      UPrimitiveComponent* OutLookAtComponent) override;
};
