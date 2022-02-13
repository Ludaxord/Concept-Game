// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPAIController.h"

#include "GOAPGoalComponent.h"
#include "GOAPPlanner.h"
#include "GOAPTaskComponent.h"
#include "NPCBase.h"
#include "WorldStateManager.h"
#include "Kismet/GameplayStatics.h"

AGOAPAIController::AGOAPAIController(): bInterruptCurrentAction(false) {
}

void AGOAPAIController::Create(TArray<UGOAPTaskComponent*> AITasks) {

	// UE_LOG(LogTemp, Warning, TEXT("GOAP CREATE...."))
	// UE_LOG(LogTemp, Warning, TEXT("GOAP AITasks Num: %i"), AITasks.Num())

	TArray<UActorComponent*> Elements;
	GetPawn()->GetComponents(UGOAPTaskComponent::StaticClass(), Elements);

	TArray<UGOAPTaskComponent*> WorldTasks;
	for (UActorComponent* Element : Elements) {
		WorldTasks.Add(Cast<UGOAPTaskComponent>(Element));
	}

	if (StateManager == nullptr) {
		StateManager = Cast<AWorldStateManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AWorldStateManager::StaticClass()));
	}

	if (Planner == nullptr) {
		Planner = Cast<AGOAPPlanner>(UGameplayStatics::GetActorOfClass(GetWorld(), AGOAPPlanner::StaticClass()));
	}

	if (StateManager == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("GOAP: failed to Get WorldStateManager..."));
	}

	if (Planner == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("GOAP: failed to Get AGOAPPlanner..."));
	}

	Tasks.Append(AITasks);
	bInvoked = false;
}

void AGOAPAIController::CompleteTask() {
	UE_LOG(LogTemp, Error, TEXT("GOAP Complete Task: %s"), *CurrentTask->GetName())
	CurrentTask->bRunning = false;
	bool bPostPerformed = CurrentTask->PostPerform();
	UE_LOG(LogTemp, Error, TEXT("GOAP Task PostPerformed: %s"), bPostPerformed ? TEXT("true") : TEXT("false"))
	bInvoked = false;

	if (CurrentGoal != nullptr) {
		// for (CurrentTask->Effects) {
		// CurrentGoal->Goals;
		// }
	}
}

void AGOAPAIController::Update() {
	UE_LOG(LogTemp, Warning,
	       TEXT("========================= Update GOAP Actor: %s... TasksQueue Num: %i ========================="),
	       *GetPawn()->GetName(),
	       TasksQueue.Num())

	//TODO: Add states based on this docs....
	//http://alumni.media.mit.edu/~jorkin/GOAP_draft_AIWisdom2_2003.pdf
	//http://alumni.media.mit.edu/~jorkin/gdc2006_orkin_jeff_fear.pdf
	//https://www.aaai.org/ocs/index.php/AIIDE/AIIDE14/paper/view/8999
	//http://alumni.media.mit.edu/~jorkin/goap.html
	//https://www.youtube.com/watch?v=gm7K68663rA
	//https://www.youtube.com/watch?v=jUSrVF8mve4
	//https://medium.com/@t2thompson/the-ai-of-fear-76ba26dc1667
	//https://www.youtube.com/watch?v=PaOLBOuyswI
	//https://www.cs.rochester.edu/~brown/242/assts/termprojs/games.pdf

	States = StateManager->GetStates();


	//
	// if (CurrentTask != nullptr) {
	// 	UE_LOG(LogTemp, Warning, TEXT("GOAP NPC -> %s CurrentTask: %s Is Running: %s Distance: %f Range: %f"),
	// 	       *GetPawn()->GetName(),
	// 	       *CurrentTask->GetName(),
	// 	       CurrentTask->bRunning ? TEXT("true") : TEXT("false"),
	// 	       FVector::Distance(GetPawn()->GetActorLocation(), CurrentTask->GetTarget()),
	// 	       CurrentTask->GetRange())
	// }

	if (CurrentTask != nullptr && CurrentTask->bRunning) {
		if (FVector::Distance(GetPawn()->GetActorLocation(), CurrentTask->GetTarget()) < CurrentTask->GetRange()) {
			StopMovement();
			CompleteTask();
		}
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("GOAP TasksQueue.Num : %i Goals : %i States : %i"), TasksQueue.Num(), Goals.Num(),
	       States.Num())

	if (TasksQueue.Num() <= 0) {
		for (TTuple<UGOAPGoalComponent*, int> Goal : Goals) {

			if (Goal.Key != nullptr) {
				//TODO: If set Goal.Key->Goals, Task stops working... need to check it...
				TasksQueue = Planner->GetPlan(
					Tasks,
					Goal.Key->Goals /* FIX Goals not adding tasks if new are visible */,
					States
				);

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

	if (CurrentTask != nullptr) {
		SetCurrentNPCStateDelegate.Broadcast(CurrentTask->GetTaskName());
	}

}

void AGOAPAIController::InitGoals(ANPCBase* NPC) {
	int32 index = 0;
	for (UGOAPGoalComponent* Goal : NPC->InitGoals_Implementation()) {
		Goals.Add(Goal, index);
		index++;
	}
}

void AGOAPAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	if (ANPCBase* NPC = Cast<ANPCBase>(InPawn)) {
		NPC->SetGoals();
		InitGoals(NPC);
		TArray<UGOAPTaskComponent*> NPCTasks;
		NPC->GetComponents(NPCTasks);
		NPC->AttachActorsToGOAP();
		Create(NPCTasks);
		GetWorldTimerManager().SetTimer(OnUpdateTimer, this, &AGOAPAIController::Update, 1.f, true, 0);
	}
}
