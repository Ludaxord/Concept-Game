// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPTaskComponent.h"
#include "GOAPDialogTaskComponent.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UGOAPDialogTaskComponent : public UGOAPTaskComponent {
	GENERATED_BODY()

public:
	UGOAPDialogTaskComponent();

	virtual void BeginPlay() override;

	virtual void CallActors() override;

	UFUNCTION(BlueprintCallable)
	virtual bool PrePerform() override;

	UFUNCTION(BlueprintCallable)
	virtual bool PostPerform() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsViable() override;

	virtual bool StartAction() override;

	virtual bool PerformAction() override;
};
