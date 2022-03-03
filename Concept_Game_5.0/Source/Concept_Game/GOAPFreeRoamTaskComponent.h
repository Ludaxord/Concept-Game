// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPTaskComponent.h"
#include "GOAPFreeRoamTaskComponent.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UGOAPFreeRoamTaskComponent : public UGOAPTaskComponent {
	GENERATED_BODY()
public:
	UGOAPFreeRoamTaskComponent();

	virtual void BeginPlay() override;

	virtual void CallActors() override;

	UFUNCTION(BlueprintCallable)
	virtual bool PrePerform() override;

	UFUNCTION(BlueprintCallable)
	virtual bool PostPerform() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsViable() override;
};
