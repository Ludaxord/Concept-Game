// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPNode.h"
#include "GameFramework/Actor.h"
#include "GOAPPlanner.generated.h"

//TODO Change AActor to UObject...
UCLASS()
class CONCEPT_GAME_API AGOAPPlanner : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGOAPPlanner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<class UGOAPTaskComponent*> GetPlan(TArray<UGOAPTaskComponent*> InTasks, const TMap<FString, int32>& InGoals,
	                                          const TMap<FString, int32>& InStates);

private:
	bool BuildGraph(GOAPNode* Parent, TArray<GOAPNode*>& InNodes, TArray<UGOAPTaskComponent*> PossibleTasks,
	                TMap<FString, int32> InGoals);

	TArray<UGOAPTaskComponent*> TaskSubset(TArray<UGOAPTaskComponent*>& InTasks, UGOAPTaskComponent* TaskToRemove);

	TArray<GOAPNode*> Nodes;

	bool GoalReached(TMap<FString, int32> Goal, TMap<FString, int32> State);

	class AWorldStateManager* StateManager;
};
