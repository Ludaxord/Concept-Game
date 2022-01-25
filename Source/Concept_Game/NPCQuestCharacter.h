// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "QuestsComponent.h"
#include "NPCQuestCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ANPCQuestCharacter : public ANPCBase {
	GENERATED_BODY()

public:
	ANPCQuestCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void Interact_Implementation() override;

private:
	void LoadQuestsToNPC();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "NPC", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* QuestIndicatorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Quests", meta = (AllowPrivateAccess = "true"))
	FNPCQuestStore Quests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC", meta = (AllowPrivateAccess = "true"))
	FString NPCName;
};
