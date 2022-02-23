// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestHolderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestHolderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CONCEPT_GAME_API IQuestHolderInterface
{
	GENERATED_BODY()

public:		
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool QuestAvailable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void QuestInteract(class AMainCharacter* InCharacter);

	UFUNCTION(BlueprintNativeEvent)
	 void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
};
