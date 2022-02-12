// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

#include "GOAPDefeatEnemyGoalComponent.h"
#include "GOAPEscapeGoalComponent.h"
#include "GOAPFreeRoamGoalComponent.h"
#include "GOAPTaskComponent.h"
#include "NPCInventoryComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ANPCBase::ANPCBase(): bGoalSet(false) {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UNPCInventoryComponent>(TEXT("InventoryComponent"));

	GoalFreeRoam = CreateDefaultSubobject<UGOAPFreeRoamGoalComponent>(TEXT("GoalFreeRoam"));
	GoalDefeatEnemy = CreateDefaultSubobject<UGOAPDefeatEnemyGoalComponent>(TEXT("GoalDefeatEnemy"));
	GoalEscape = CreateDefaultSubobject<UGOAPEscapeGoalComponent>(TEXT("GoalEscape"));

	NPCSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NPCSphere"));
	NPCSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay() {
	Super::BeginPlay();

	ID = FGuid::NewGuid();

	SetGoals();
}

// Called every frame
void ANPCBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::Interact_Implementation(AMainCharacter* InCharacter) {
	INPCInterface::Interact_Implementation(InCharacter);
}

void ANPCBase::QuestInteract_Implementation(AMainCharacter* InCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("ANPCBase::QuestInteract_Implementation"))
	Interact_Implementation(InCharacter);
}

bool ANPCBase::QuestAvailable_Implementation() {
	return IQuestHolderInterface::QuestAvailable_Implementation();
}

bool ANPCBase::GoalInterrupt_Implementation(UGOAPTaskComponent* InCurrentGoal) {
	return false;
}

void ANPCBase::SetGoals() {
	if (!bGoalSet) {
		TArray<UActorComponent*> Elements;
		GetComponents(UGOAPGoalComponent::StaticClass(), Elements);
		UE_LOG(LogTemp, Display, TEXT("GOAP NPC => %s Goals Elements %i"), *GetName(), Elements.Num())

		for (UActorComponent* Element : Elements) {
			if (UGOAPGoalComponent* Goal = Cast<UGOAPGoalComponent>(Element)) {
				UE_LOG(LogTemp, Warning, TEXT("GOAP NPC => %s SetGoal : %s"), *GetName(), *Element->GetName())
				GOAPGoalComponents.Add(Goal);
				// if (Goal->bSetAsCurrentGoal) {
				// 	UE_LOG(LogTemp, Warning, TEXT("GOAP: SetGoal : %s"), *Element->GetName())
				// 	GOAPGoalComponents.Add(Goal);
				// }
			}
		}

		bGoalSet = true;
	}
}

void ANPCBase::AttachActorsToGOAP() {
	TArray<UGOAPTaskComponent*> NPCTasks;
	GetComponents(NPCTasks);

	for (UGOAPTaskComponent* Task : NPCTasks) {
		UE_LOG(LogTemp, Display, TEXT("AttachActorsToGOAP => %s"), *Task->GetName())
		Task->CallActors();
	}

}

TArray<UGOAPGoalComponent*> ANPCBase::InitGoals_Implementation() {
	return GOAPGoalComponents;
}

void ANPCBase::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult) {

}

void ANPCBase::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {

}
