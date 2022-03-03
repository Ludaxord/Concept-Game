// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionComponent.h"
#include "DialogComponent.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UDialogComponent : public UActionComponent {
	GENERATED_BODY()
public:
	UDialogComponent();

	void BeginPlay() override;


	AActor* GetDialogActor() {
		return nullptr;
	}
};
