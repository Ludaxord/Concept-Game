// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPAIController.h"

#include "GOAPGoalComponent.h"
#include "GOAPPlanner.h"
#include "GOAPTaskComponent.h"
#include "WorldStateManager.h"
#include "Kismet/GameplayStatics.h"

AGOAPAIController::AGOAPAIController() {
}

void AGOAPAIController::Create(TArray<UGOAPTaskComponent*> AITasks) {
	TArray<UActorComponent*> Elements;
	GetPawn()->GetComponents(UGOAPTaskComponent::StaticClass(), Elements);

	TArray<UGOAPTaskComponent*> WorldTasks;
	for (UActorComponent* Element : Elements) {
		WorldTasks.Add(Cast<UGOAPTaskComponent>(Element));
	}

	StateManager = Cast<AWorldStateManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AWorldStateManager::StaticClass()));

	Planner = Cast<AGOAPPlanner>(UGameplayStatics::GetActorOfClass(GetWorld(), AGOAPPlanner::StaticClass()));

	if (StateManager == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("failed to Get WorldStateManager..."));
	}

	if (Planner == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("failed to Get AGOAPPlanner..."));
	}

	Tasks.Append(AITasks);
	bInvoked = false;
}

void AGOAPAIController::CompleteTask() {
	CurrentTask->bRunning = false;
	CurrentTask->PostPerform();
	bInvoked = false;
}

void AGOAPAIController::Update() {
	States = StateManager->GetStates();

	if (CurrentTask != nullptr && CurrentTask->bRunning) {
		if (FVector::Distance(GetPawn()->GetActorLocation(), CurrentTask->GetTarget()) < CurrentTask->GetRange()) {
			StopMovement();
			CompleteTask();
		}

		return;
	}

	if (TasksQueue.Num() <= 0) {
		for (TTuple<UGOAPGoalComponent*, int> Goal : Goals) {
			if (Goal.Key != NULL) {
				TasksQueue = Planner->GetPlan(Tasks, Goal.Key->Goals, States);
				
				if (TasksQueue.Num() > 0) {
					CurrentGoal = Goal.Key;
					break;
				}
			}
		}
	}

	if (TasksQueue.Num() == 0) {
		if (CurrentGoal != nullptr) {
			if (CurrentGoal->bRemove) {
				Goals.Remove(CurrentGoal);
			}
		}
	}
	else if (TasksQueue.Num() > 0) {
		CurrentTask = TasksQueue[0];
		if (CurrentTask != nullptr) {
			if (CurrentTask->PrePerform()) {
				TasksQueue.Remove(CurrentTask);
				CurrentTask->bRunning = true;
				GetPawn()->bUseControllerRotationRoll = false;
				GetPawn()->SetActorRotation(CurrentTask->GetTarget().Rotation());
				MoveToLocation(CurrentTask->GetTarget(), false, true);
			}
			else {
				TasksQueue.Empty();
			}
		}
	}

}
