// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPTaskComponent.h"

#include "NPCBase.h"

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


// Called every frame
void UGOAPTaskComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGOAPTaskComponent::PrePerform() {
	return false;
}

bool UGOAPTaskComponent::PostPerform() {
	return false;
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
