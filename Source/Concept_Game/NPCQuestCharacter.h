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

	virtual void Interact_Implementation(AMainCharacter* InCharacter) override;

	virtual void OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                 bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

private:
	void LoadQuestsToNPC();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "NPC", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* QuestIndicatorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Quests", meta = (AllowPrivateAccess = "true"))
	FNPCQuestStore Quests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC", meta = (AllowPrivateAccess = "true"))
	FString NPCName;

public:
	FORCEINLINE FNPCQuestStore GetQuests() {
		return Quests;
	}
};
