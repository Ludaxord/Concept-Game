// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPTaskComponent.h"

#include "AIController.h"
#include "NPCBase.h"
#include "FreeRoamPoint.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGOAPTaskComponent::UGOAPTaskComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGOAPTaskComponent::BeginPlay() {
	Super::BeginPlay();

	TaskOwner = Cast<ANPCBase>(GetOwner());
}

void UGOAPTaskComponent::AttachActors(UClass* ActorClass) {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, TaskAttachedActors);
	UE_LOG(LogTemp, Warning, TEXT("GOAP NPC => %s TaskName -> %s TaskAttachedActors: %i"),
	       *GetOwner()->GetName(),
	       *GetName(),
	       TaskAttachedActors.Num())
}


bool UGOAPTaskComponent::FindNearestActorLocationFromOwner() {
	int index = 0;

	UE_LOG(LogTemp, Warning,
	       TEXT(
		       "FindNearestActorLocationFromOwner GOAP NPC => %s TaskName -> %s TaskAttachedActors => %i bLoopTaskTillBreak: %s"
	       ),
	       *GetOwner()->GetName(),
	       *GetName(),
	       TaskAttachedActors.Num(),
	       bLoopTaskTillBreak ? TEXT("true") : TEXT("false"))
	if (TaskAttachedActors.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("GOAP NPC => %s Scanning Actors"), *GetOwner()->GetName())
		if (bLoopTaskTillBreak) {
			for (AActor* AttachedActor : TaskAttachedActors) {
				if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(AttachedActor)) {
					if (GOAPActor != NearestAttachedActor) {
						GOAPActor->DisabledActors.Remove(GetOwner());
					}
					else {
						GOAPActor->DisabledActors.Add(GetOwner());
					}
				}
			}
			TaskAttachedActors.Remove(NearestAttachedActor);
		}
		else {
			NearestAttachedActor = nullptr;
		}
	}

	for (AActor* AttachedActor : TaskAttachedActors) {
		if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(AttachedActor)) {
			if (GOAPActor->DisabledActors.Contains(GetOwner())) {
				continue;
			}

		}
		if (index == 0) {
			NearestAttachedActor = AttachedActor;
		}
		else {
			if (AttachedActor->GetActorLocation().X < NearestAttachedActor->GetActorLocation().X ||
				AttachedActor->GetActorLocation().Y < NearestAttachedActor->GetActorLocation().Y ||
				AttachedActor->GetActorLocation().Z < NearestAttachedActor->GetActorLocation().Z) {
				NearestAttachedActor = AttachedActor;
			}
		}
		UE_LOG(LogTemp, Warning,
		       TEXT("GOAP FindNearestActorLocationFromOwner GOAP NPC => %s Task Location: %s Owner Location: %s"),
		       *GetOwner()->GetName(),
		       *AttachedActor->GetActorLocation().ToString(),
		       *TaskOwner->GetActorLocation().ToString()
		)
		index++;
	}

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Orange, TEXT("")
	);

	if (NearestAttachedActor != nullptr) {
		FNavLocation Loc;
		UNavigationSystemV1::GetNavigationSystem(GetWorld())->
			GetRandomPointInNavigableRadius(NearestAttachedActor->GetActorLocation(), Range, Loc);


		const FString TheFloatStr = FString::SanitizeFloat(Range);
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green,
		                                 TEXT("NearestAttachedActor ") + NearestAttachedActor->GetName() + TEXT(" Loc ")
		                                 +
		                                 NearestAttachedActor->GetActorLocation().ToString() +
		                                 TEXT(" Range ") + TheFloatStr + TEXT(" Random ") + Loc.Location.ToString()
		);

		if (UNavigationSystemV1::GetNavigationSystem(GetWorld())->
			GetRandomPointInNavigableRadius(NearestAttachedActor->GetActorLocation(), Range, Loc)) {

			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald,
			                                 TEXT("GetRandomPointInNavigableRadius FOR ") + NearestAttachedActor->
			                                 GetName() + TEXT(" AT RANGE ") + FString::FromInt(Range)
			);
			Target = Loc.Location;
			if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(NearestAttachedActor)) {
				GOAPActor->DisabledActors.Add(GetOwner());
				TaskAttachedActors.Remove(NearestAttachedActor);
			}

			return true;
		}
	}
	else {

		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("NearestAttachedActor IS NULL")
		);
	}

	return false;
}

bool UGOAPTaskComponent::FindFromAttachedActors() {
	if (NPCTargetActorPoints.Num() > 0) {
		for (AActor* TargetActor : NPCTargetActorPoints) {
			if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(TargetActor)) {
				if (GOAPActor != NearestAttachedActor) {
					NearestAttachedActor = GOAPActor;
					return true;
				}
			}
		}
	}

	return false;
}

void UGOAPTaskComponent::CallActors() {
}

// Called every frame
void UGOAPTaskComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGOAPTaskComponent::PrePerform() {
	if (NPCTargetActorPoints.Num() > 0) {
		return FindFromAttachedActors();
	}

	return FindNearestActorLocationFromOwner();
}

bool UGOAPTaskComponent::PostPerform() {
	if (ANPCBase* NPC = Cast<ANPCBase>(GetOwner())) {
		if (NPC->bUpdateGoals) {
			return true;
		}
	}

	if (NPCTargetActorPoints.Num() > 0) {
		return FindFromAttachedActors();
	}

	return FindNearestActorLocationFromOwner();
}

bool UGOAPTaskComponent::DuringPerform() {
	return false;
}

bool UGOAPTaskComponent::PerformAction() {
	return FVector::Distance(TaskOwner->GetAIController()->GetPawn()->GetActorLocation(), GetTarget()) < GetRange();
}

bool UGOAPTaskComponent::IsViable() {
	return bIsViableTask;
}

bool UGOAPTaskComponent::GoalInterrupt(UGOAPTaskComponent* InCurrentGoal) {
	if (TaskOwner) {
		if (TaskOwner->GoalInterrupt_Implementation(InCurrentGoal)) {
			return true;
		}
	}

	return false;
}

bool UGOAPTaskComponent::IsViableGiven(const TMap<FString, int32> InConditions) {
	for (const TTuple<FString, int32> PreCondition : PreConditions) {
		if (!InConditions.Contains(PreCondition.Key)) {
			return false;
		}
	}

	return true;
}

bool UGOAPTaskComponent::StartAction() {
	TaskOwner->GetAIController()->GetPawn()->bUseControllerRotationRoll = false;
	TaskOwner->GetAIController()->GetPawn()->SetActorRotation(GetTarget().Rotation());
	TaskOwner->GetAIController()->MoveToLocation(GetTarget(), false, true);

	return true;
}
