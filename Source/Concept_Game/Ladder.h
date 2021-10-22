// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ALadder : public AInteractiveItem {
	GENERATED_BODY()
public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	int RungsNumber;
};
