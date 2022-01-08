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

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	virtual bool InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) override;

	virtual AActor* LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	                                      APlayerController* InController, FText& OutMessage,
	                                      UPrimitiveComponent* OutLookAtComponent) override;

	virtual void OnPhysicsInteraction();

	virtual void OnLiftItem();

	virtual void OnDropItem();

	virtual void OnThrowItem();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PhysicsBasedMesh;
};
