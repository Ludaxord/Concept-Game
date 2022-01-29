// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CONCEPT_GAME_API INPCInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AMainCharacter* InCharacter);
};
