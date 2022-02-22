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
                                                  const TMap<FString, int32>& InStates,
                                                  APawn* OwnerPawn) {
	Nodes.Empty();

	GOAPNode* BeginNode = new GOAPNode(nullptr, 0.f, StateManager->GetStates(), InStates, nullptr);
	bool bSuccess = BuildGraph(BeginNode, Nodes, InTasks, InGoals);
	// bool bSuccess = BuildGraph(BeginNode, Nodes, CurrentTasks, InGoals);


	if (OwnerPawn != nullptr) {
		if (!bSuccess) {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			                                 TEXT("Planning failed, no plan found for Pawn: ") + OwnerPawn->GetName()
			);
			UE_LOG(LogTemp, Error, TEXT("Planning failed, no plan found for Pawn: %s"), *OwnerPawn->GetName());
		}
		else {
			// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
			//                                  TEXT("Plan build Tasks: ") + FString::FromInt(InTasks.Num()) + TEXT(
			// 	                                 " Goals: ")
			//                                  + FString::FromInt(InGoals.Num()) + TEXT(" States: ") + FString::FromInt(
			// 	                                 InStates.Num()) + TEXT(" For Pawn: ") + OwnerPawn->GetName());

			UE_LOG(LogTemp, Warning, TEXT("Plan build Tasks: %i, Goals: %i, States: %i, For Pawn: %s"), InTasks.Num(),
			       InGoals.Num(), InStates.Num(), *OwnerPawn->GetName());
		}
	}

	if (!bSuccess)
		return {};

	GOAPNode* CheapestNode = new GOAPNode();
	CheapestNode->Cost = HUGE_VALF;

	if (Nodes.Num() > 0) {
		for (GOAPNode* Node : Nodes) {
			// UE_LOG(LogTemp, Warning, TEXT("++++++++ Node Task : %s == Cost: %f ++++++++"), *Node->Task->GetName(),
			//        Node->Cost)
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
		UE_LOG(LogTemp, Warning, TEXT("UGOAPTaskComponent Added To Plan :> %s From Owner: %s"), *Task->GetName(),
		       *Task->GetOwner()->GetName())
		Plan.Add(Task);
	}

	UE_LOG(LogTemp, Warning, TEXT("GOAP: Planning for %i Nodes Completed"), Plan.Num());

	return Plan;
}

bool AGOAPPlanner::BuildGraph(GOAPNode* Parent, TArray<GOAPNode*>& InNodes, TArray<UGOAPTaskComponent*> PossibleTasks,
                              TMap<FString, int32> InGoals) {
	bool bPathExists = false;

	for (UGOAPTaskComponent* const& Task : PossibleTasks) {
		if (Task->IsViableGiven(Parent->States)) {
			TMap<FString, int32> CurrentState = {Parent->States};
			for (TTuple<FString, int32> const& Effect : Task->Effects) {
				if (!CurrentState.Contains(Effect.Key)) {
					CurrentState.Add(Effect.Key, Effect.Value);
				}
			}

			GOAPNode* Node = new GOAPNode(Parent, Parent->Cost + Task->Cost, CurrentState, Task);

			// if (GoalReached(InGoals, CurrentState)) {
			if (AllGoalsReached(InGoals, CurrentState)) {
				InNodes.Add(Node);
				UE_LOG(LogTemp, Warning, TEXT("%s -> GoalReached"), *Task->GetOwner()->GetName())
				bPathExists = true;
			}
			else {
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
			UE_LOG(LogTemp, Warning, TEXT("GOAP %s ActionSubset: %s"), *Task->GetOwner()->GetName(), *Task->GetName())
			TasksSubset.Add(Task);
		}
	}

	return TasksSubset;
}

bool AGOAPPlanner::GoalReached(TMap<FString, int32> Goal, TMap<FString, int32> State) {
	for (TTuple<FString, int32> const& SubGoal : Goal) {
		UE_LOG(LogTemp, Error, TEXT("GOAP Goal %s in state: %s"), *SubGoal.Key,
		       State.Contains(SubGoal.Key) ? TEXT("true") : TEXT("false"))
		if (!State.Contains(SubGoal.Key)) {
			return false;
		}
	}

	return true;
}

bool AGOAPPlanner::AllGoalsReached(TMap<FString, int32> Goal, TMap<FString, int32> State) {
	int FoundGoals = 0;
	for (TTuple<FString, int32> const& SubGoal : Goal) {
		for (TTuple<FString, int32> const& SubState : State) {
			if (SubGoal.Key == SubState.Key) {
				if (SubState.Value >= SubGoal.Value) {
					FoundGoals++;
				}
			}
		}

	}

	return Goal.Num() == FoundGoals;
}
