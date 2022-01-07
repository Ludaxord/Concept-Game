// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "PhysicsInteractionItem.generated.h"

UCLASS()
class CONCEPT_GAME_API APhysicsInteractionItem : public AActor, public IPhysicsBasedInteractionInterface {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsInteractionItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) override;

	virtual AActor* LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
	                                      APlayerController* InController, FText& OutMessage,
	                                      UPrimitiveComponent* OutLookAtComponent) override;

};
