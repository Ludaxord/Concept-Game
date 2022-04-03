// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTreeAIController.h"

#include "NavigationSystem.h"
#include "NPCBase.h"
#include "TeamInfoInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABehaviorTreeAIController::ABehaviorTreeAIController() {
	SightPerceptionComponent = CreateDefaultSubobject<UBaseAIPerceptionComponent>(TEXT("SightPerceptionComponent"));
	HearPerceptionComponent = CreateDefaultSubobject<UBaseAIPerceptionComponent>(TEXT("HearPerceptionComponent"));
}

void ABehaviorTreeAIController::TargetPerceptionUpdated(AActor* InActor, FAIStimulus InStimulus) {
	UE_LOG(LogTemp, Warning, TEXT("====================== TargetPerceptionUpdated ======================"))
	if (InStimulus.WasSuccessfullySensed()) {
		if (IsVisibleActorEnemy(InActor)) {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
				UpdateTargetActor();
			}, 1.f, false);
		}
		else {
			Blackboard->ClearValue(FName("TargetActor"));
			TargetActor = nullptr;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
				UpdateTargetActor();
			}, 1.f, false);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
				if (!IsValid(TargetActor)) {
					LockActor = nullptr;
				}
			}, 20.f, true);
		}
	}
}

bool ABehaviorTreeAIController::GetRandomLocationForActor(float InRadius, FVector& OutVector) {

	if (IsValid(GetPawn())) {
		FNavLocation Loc;
		bool bRes = UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomReachablePointInRadius(
			GetPawn()->GetActorLocation(), InRadius, Loc);

		OutVector = Loc.Location;

		return bRes;
	}

	return false;
}

bool ABehaviorTreeAIController::SetActorFocus(AActor* InFocusActor) {
	// if (IsVisibleActorEnemy(InFocusActor)) {
	if (InFocusActor) {
		SetFocus(InFocusActor);
		return true;
	}

	ClearFocus(EAIFocusPriority::Default);
	return false;
}

bool ABehaviorTreeAIController::ClearActorFocus() {
	ClearFocus(EAIFocusPriority::Default);
	return true;
}

bool ABehaviorTreeAIController::IsVisibleActorEnemy(AActor* InTargetActor) {
	if (InTargetActor) {
		if (InTargetActor->GetClass()->ImplementsInterface(UTeamInfoInterface::StaticClass())) {
			ITeamInfoInterface* CurrentTargetActor = Cast<ITeamInfoInterface>(InTargetActor);
			if (ActorDetected(CurrentTargetActor)) {
				return CurrentTargetActor->Execute_IsAlive(this);
			}
		}
	}

	return false;
}

bool ABehaviorTreeAIController::ActorDetected(ITeamInfoInterface* CurrentTargetActor) {
	return CurrentTargetActor->IsTargetAnEnemy(CurrentTargetActor->Execute_GetTeamID(this));
}

AActor* ABehaviorTreeAIController::GetClosestEnemy() {
	AActor* ClosestActor = nullptr;
	float Distance = 100.f;
	TArray<AActor*> OutActors;
	SightPerceptionComponent->GetCurrentlyPerceivedActors(SightPerceptionComponent->GetDominantSense(), OutActors);

	for (AActor* Actor : OutActors) {
		if (IsVisibleActorEnemy(Actor)) {
			if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), GetPawn()->GetActorLocation()) <
				Distance) {
				ClosestActor = Actor;
			}
		}
	}
	return ClosestActor;
}

void ABehaviorTreeAIController::UpdateTargetActor() {
	AActor* ClosestEnemy = GetClosestEnemy();
	TargetActor = ClosestEnemy;
	LockActor = ClosestEnemy;

	if (IsValid(TargetActor)) {
		Blackboard->SetValueAsObject(FName("TargetActor"), TargetActor);
		Blackboard->SetValueAsObject(FName("LockedActor"), LockActor);
	}
}

void ABehaviorTreeAIController::FireWeapon() {
	if (GetPawn() != nullptr) {
		if (IsValid(GetPawn())) {
			const float RandomDelay = FMath::FRandRange(3.f, 6.f);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
				ANPCBase* NPC = Cast<ANPCBase>(GetPawn());
				if (!NPC->GetEquippedWeapon()) {
					NPC->NPCEquipWeapon(EInventoryWeapon::EIW_Any);
				}

				NPC->FireWeapon_Implementation();
			}, RandomDelay, true);

		}
	}
}

void ABehaviorTreeAIController::BeginPlay() {
	Super::BeginPlay();
	SightPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
		this, &ABehaviorTreeAIController::TargetPerceptionUpdated);
}

void ABehaviorTreeAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	if (CurrentBehaviorTree) {
		RunBehaviorTree(CurrentBehaviorTree);
	}
}
