// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PhysicsBasedInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPhysicsBasedInteractionInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class CONCEPT_GAME_API IPhysicsBasedInteractionInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool InteractWith(AActor* InActor, UPrimitiveComponent* InHitComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* LookAt(AActor* InActor, UPrimitiveComponent* InHitComponent, APlayerController* InController,
	               FText& OutMessage, UPrimitiveComponent* OutLookAtComponent);
};
