// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "MiniMapSkill.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UMiniMapSkill : public USkill
{
	GENERATED_BODY()
public:
	virtual void ApplySkill(AActor* InOwner) override;

	UFUNCTION(BlueprintCallable)
	virtual void Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount,
	                  UTexture2D* InImage) override;
	
};
