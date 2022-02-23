// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CONCEPT_GAME_API WorldState
{
public:
	WorldState();
	WorldState(FString InKey, int32 InValue);
	~WorldState();

	FString Key;
	int32 Value;
};
