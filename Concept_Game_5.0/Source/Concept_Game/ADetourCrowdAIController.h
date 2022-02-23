// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ADetourCrowdAIController.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AADetourCrowdAIController : public AAIController
{
	GENERATED_BODY()
public:
	AADetourCrowdAIController(const FObjectInitializer& ObjInitializer = FObjectInitializer::Get());
};
