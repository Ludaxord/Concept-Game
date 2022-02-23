// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPTaskComponent.h"

/**
 * 
 */
class CONCEPT_GAME_API GOAPNode {
public:
	GOAPNode() = default;

	GOAPNode(GOAPNode* InParent, float InCost, TMap<FString, int32> States, UGOAPTaskComponent* InTask);

	GOAPNode(GOAPNode* InParent, float InCost, const TMap<FString, int32>& AllStates,
	         const TMap<FString, int32>& InStates, UGOAPTaskComponent* InTask);

	~GOAPNode();

public:
	GOAPNode* Parent = nullptr;

	float Cost = 0.0f;

	TMap<FString, int32> States;

	UGOAPTaskComponent* Task = nullptr;
};
