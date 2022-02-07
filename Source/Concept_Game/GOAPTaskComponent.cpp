// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPTaskComponent.h"

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
	UE_LOG(LogTemp, Warning, TEXT("GOAP TaskAttachedActors: %i"), TaskAttachedActors.Num())
}


bool UGOAPTaskComponent::FindNearestActorLocationFromOwner() {
	int index = 0;
	UE_LOG(LogTemp, Warning, TEXT("GOAP TaskAttachedActors => %i bLoopTaskTillBreak: %s"), TaskAttachedActors.Num(),
	       bLoopTaskTillBreak ? TEXT("true") : TEXT("false"))
	if (TaskAttachedActors.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("GOAP Scanning Actors"))
		if (bLoopTaskTillBreak) {
			AttachActors(AFreeRoamPoint::StaticClass());
			for (AActor* AttachedActor : TaskAttachedActors) {
				if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(AttachedActor)) {
					if (GOAPActor != NearestAttachedActor) {
						GOAPActor->bEnabled = true;
					}
					else {
						GOAPActor->bEnabled = false;
					}
					UE_LOG(LogTemp, Warning, TEXT("GOAP Actor => %s Name -> %s"), *GOAPActor->GetName(),
					       GOAPActor->bEnabled ? TEXT("true") : TEXT("false"))
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
			if (!GOAPActor->bEnabled) {
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
		UE_LOG(LogTemp, Warning, TEXT("GOAP FindNearestActorLocationFromOwner Task Location: %s Owner Location: %s"),
		       *AttachedActor->GetActorLocation().ToString(), *TaskOwner->GetActorLocation().ToString())
		index++;
	}

	if (NearestAttachedActor != nullptr) {
		FNavLocation Loc;
		if (UNavigationSystemV1::GetNavigationSystem(GetWorld())->
			GetRandomPointInNavigableRadius(NearestAttachedActor->GetActorLocation(), Range, Loc)) {
			Target = Loc.Location;
			if (AGOAPTaskAttachedActor* GOAPActor = Cast<AGOAPTaskAttachedActor>(NearestAttachedActor)) {
				GOAPActor->bEnabled = false;
				TaskAttachedActors.Remove(NearestAttachedActor);
			}

			return true;
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
	return FindNearestActorLocationFromOwner();
}

bool UGOAPTaskComponent::PostPerform() {
	return FindNearestActorLocationFromOwner();
}

bool UGOAPTaskComponent::IsViable() {
	return false;
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
