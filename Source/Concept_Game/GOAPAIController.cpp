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
	// UE_LOG(LogTemp, Warning, TEXT("GOAP Complete Task: %s"), *CurrentTask->GetName())
	CurrentTask->bRunning = false;
	bool bPostPerformed = CurrentTask->PostPerform();
	// UE_LOG(LogTemp, Error, TEXT("GOAP Task PostPerformed: %s"), bPostPerformed ? TEXT("true") : TEXT("false"))
	bInvoked = false;
}

void AGOAPAIController::Update() {
	UE_LOG(LogTemp, Warning, TEXT("Update GOAP... TasksQueue Num: %i"), TasksQueue.Num())

	if (CurrentTask != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("GOAP CurrentTask Name: %s Is Running: %s"), *CurrentTask->GetName(),
		       CurrentTask->bRunning ? TEXT("true"): TEXT("false"))
	}

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

	//TODO: FIX!!!!
	// if (StateManager->GetInterruptCurrentState()) {
	// 	if (States.Num() < StateManager->GetStates().Num()) {
	// 		UE_LOG(LogTemp, Warning, TEXT("GOAP State Changed... States Before %i ... States After %i"), States.Num(),
	// 		       StateManager->GetStates().Num())
	// 		if (CurrentTask != nullptr) {
	// 			CompleteTask();
	// 			CurrentTask->bIsViableTask = false;
	// 			StateManager->SetInterruptCurrentState(false);
	// 		}
	// 		for (auto State : StateManager->GetStates()) {
	// 			UE_LOG(LogTemp, Warning, TEXT("GOAP State: %s"), *State.Key)
	// 			if (!States.Contains(State.Key)) {
	// 			}
	// 		}
	// 	}
	// }

	States = StateManager->GetStates();

	if (CurrentTask != nullptr && CurrentTask->bRunning) {
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f, Range: %f"),
		       FVector::Distance(GetPawn()->GetActorLocation(), CurrentTask->GetTarget()), CurrentTask->GetRange())

		if (FVector::Distance(GetPawn()->GetActorLocation(), CurrentTask->GetTarget()) < CurrentTask->GetRange()) {
			StopMovement();
			CompleteTask();
		}
		return;
	}

	if (TasksQueue.Num() <= 0) {
		// UE_LOG(LogTemp, Warning, TEXT("GOAP TasksQueue.Num : %i Goals : %i"), TasksQueue.Num(), Goals.Num())
		for (TTuple<UGOAPGoalComponent*, int> Goal : Goals) {
			// UE_LOG(LogTemp, Error, TEXT("GOAP GOAL: %s Indx: %i"), *Goal.Key->GetName(), Goal.Value);
			if (Goal.Key != NULL) {
				//TODO: If set Goal.Key->Goals, Task stops working... need to check it...
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
			// UE_LOG(LogTemp, Warning, TEXT("GOAP CurrentTask TasksQueue %s"), *CurrentTask->GetName())
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

void AGOAPAIController::InitGoals(ANPCBase* NPC) {
	// UE_LOG(LogTemp, Warning, TEXT("GOAP AGOAPAIController::InitGoals : %i"), NPC->InitGoals_Implementation().Num())
	int32 index = 0;
	for (UGOAPGoalComponent* Goal : NPC->InitGoals_Implementation()) {
		// UE_LOG(LogTemp, Warning, TEXT("GOAP InitGoal_Implementation: %s"), *Goal->GetName())
		Goals.Add(Goal, index);
		index++;
	}
}

void AGOAPAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	// UE_LOG(LogTemp, Warning, TEXT("GOAP ON POSSESS "))
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
