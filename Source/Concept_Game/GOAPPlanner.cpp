// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPPlanner.h"
#include "WorldStateManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AGOAPPlanner::AGOAPPlanner() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGOAPPlanner::BeginPlay() {
	Super::BeginPlay();

	StateManager = Cast<AWorldStateManager>(
		UGameplayStatics::GetActorOfClass(AActor::GetWorld(), AWorldStateManager::StaticClass()));
}

// Called every frame
void AGOAPPlanner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

TArray<UGOAPTaskComponent*> AGOAPPlanner::GetPlan(TArray<UGOAPTaskComponent*> InTasks,
                                                  const TMap<FString, int32>& InGoals,
                                                  const TMap<FString, int32>& InStates) {
	Nodes.Empty();

	TArray<UGOAPTaskComponent*> CurrentTasks;

	UE_LOG(LogTemp, Error, TEXT("GOAP Planning Tasks Num: %i"), InTasks.Num())

	for (UGOAPTaskComponent* const& Task : InTasks) {
		UE_LOG(LogTemp, Warning, TEXT("GOAP Planning Task: %s"), *Task->GetName())
		if (Task->IsViable()) {
			CurrentTasks.Add(Task);
		}
	}

	GOAPNode* BeginNode = new GOAPNode(nullptr, 0.f, StateManager->GetStates(), InStates, nullptr);
	// bool bSuccess = BuildGraph(BeginNode, Nodes, InTasks, InGoals);
	bool bSuccess = BuildGraph(BeginNode, Nodes, CurrentTasks, InGoals);

	if (!bSuccess) {
		UE_LOG(LogTemp, Error, TEXT("GOAP: No Plans found..."))
		return {};
	}

	GOAPNode* CheapestNode = new GOAPNode();
	CheapestNode->Cost = HUGE_VALF;

	if (Nodes.Num() > 0) {
		for (GOAPNode* Node : Nodes) {
			if (Node->Cost < CheapestNode->Cost) {
				CheapestNode = Node;
			}
		}
	}

	TArray<UGOAPTaskComponent*> TasksResults;
	GOAPNode* Node = CheapestNode;

	while (Node != nullptr) {
		if (Node->Task != nullptr) {
			TasksResults.Insert(Node->Task, 0);
		}

		Node = Node->Parent;
	}

	TArray<UGOAPTaskComponent*> Plan = {};

	for (UGOAPTaskComponent* const& Task : TasksResults) {
		Plan.Add(Task);
	}

	// UE_LOG(LogTemp, Warning, TEXT("GOAP: Planning for %i nodes Completed"), Plan.Num());

	return Plan;
}

bool AGOAPPlanner::BuildGraph(GOAPNode* Parent, TArray<GOAPNode*>& InNodes, TArray<UGOAPTaskComponent*> PossibleTasks,
                              TMap<FString, int32> InGoals) {
	UE_LOG(LogTemp, Error, TEXT("Build Graph"))

	UE_LOG(LogTemp, Warning, TEXT("Parent Exists: %s"), Parent ? TEXT("true") : TEXT("false"))

	bool bPathExists = false;

	for (UGOAPTaskComponent* const& Task : PossibleTasks) {
		UE_LOG(LogTemp, Error, TEXT("Build Graph PossibleTask : %s"), *Task->GetName())
		if (Task->IsViableGiven(Parent->States)) {
			TMap<FString, int32> CurrentState = {Parent->States};
			UE_LOG(LogTemp, Error, TEXT("Build Graph CurrentState : %i"), CurrentState.Num())
			for (TTuple<FString, int32> const& Effect : Task->Effects) {
				if (!CurrentState.Contains(Effect.Key)) {
					CurrentState.Add(Effect.Key, Effect.Value);
				}
			}

			GOAPNode* Node = new GOAPNode(Parent, Parent->Cost + Task->Cost, CurrentState, Task);

			if (GoalReached(InGoals, CurrentState)) {
				InNodes.Add(Node);
				bPathExists = true;
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Goal NOT Reached.... "))
				TArray<UGOAPTaskComponent*> Subset = TaskSubset(PossibleTasks, Task);
				bool bFound = BuildGraph(Node, InNodes, Subset, InGoals);
				if (bFound) {
					bPathExists = true;
				}
			}
		}
	}

	return bPathExists;
}

TArray<UGOAPTaskComponent*> AGOAPPlanner::TaskSubset(TArray<UGOAPTaskComponent*>& InTasks,
                                                     UGOAPTaskComponent* TaskToRemove) {
	TArray<UGOAPTaskComponent*> TasksSubset;

	for (UGOAPTaskComponent* const& Task : InTasks) {
		if (Task != TaskToRemove) {
			TasksSubset.Add(Task);
		}
	}

	return TasksSubset;
}

bool AGOAPPlanner::GoalReached(TMap<FString, int32> Goal, TMap<FString, int32> State) {
	for (TTuple<FString, int32> const& SubGoal : Goal) {
		UE_LOG(LogTemp, Error, TEXT("GOAP: SubGoal -> %s : %i"), *SubGoal.Key, SubGoal.Value)
		for (auto S : State) {
			UE_LOG(LogTemp, Error, TEXT("GOAP: State -> %s : %i"), *S.Key, S.Value)
		}

		if (!State.Contains(SubGoal.Key)) {
			return false;
		}
	}

	return true;
}
