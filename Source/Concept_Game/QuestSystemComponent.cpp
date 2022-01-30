// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemComponent.h"

#include "MainCharacter.h"
#include "NPCQuestCharacter.h"
#include "QuestHolderInterface.h"

// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSystemComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());

	AddRemoveQuestDelegate.AddDynamic(this, &UQuestSystemComponent::UpdateCache);
	AskForQuestDelegate.AddDynamic(this, &UQuestSystemComponent::AskForQuest);
	AcceptQuestDelegate.AddDynamic(this, &UQuestSystemComponent::AcceptQuest);
}


// Called every frame
void UQuestSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForQuestsHolders();
}

void UQuestSystemComponent::AddQuest(FNPCQuest InNPCQuest) {
	FQuest Quest = {InNPCQuest.Name, InNPCQuest.Description, InNPCQuest.QuestSteps, InNPCQuest.bPrimary};
	Quests.Add(Quest);

	if (AddRemoveQuestDelegate.IsBound()) {
		AddRemoveQuestDelegate.Broadcast();

		if (Quest.bPrimary && bCanChangeQuest) {
			SelectQuest(Quest.Name);
		}
	}
}

void UQuestSystemComponent::SelectQuest(FString InName) {
	if (Quests.Num() > 0) {
		if (ActiveQuest.Name != InName) {
			ResetQuest();
			if (ActiveQuest.Name == "") {
				CurrentQuestID = 0;
				ActiveQuest = Quests[CurrentQuestID];
			}
			else {
				for (int i = 0; i < Quests.Num(); i++) {
					if (Quests.IsValidIndex(i)) {
						if (InName == Quests[i].Name) {
							CurrentQuestID = i;
							ActiveQuest = Quests[i];
							break;
						}
					}
				}
			}
		}
	}
	else {
		ResetQuest();
	}
}

void UQuestSystemComponent::RemoveQuest() {
	Quests.RemoveAt(CurrentQuestID);
	SelectQuest(FString());
	bCanChangeQuest = true;
}

void UQuestSystemComponent::CompleteQuestStep() {
	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num() == 0) {
		RemoveQuest();
	}
	else {
		bCanChangeQuest = false;
		UpdateCache();
	}
}

void UQuestSystemComponent::QuestListToggle() {
	bQuestListVisible = !bQuestListVisible;
	UE_LOG(LogTemp, Warning, TEXT("bQuestList Visible: %s"), bQuestListVisible ? TEXT("true"): TEXT("false"))
}

void UQuestSystemComponent::TraceForQuestsHolders() {
	if (OwningCharacter->OverlappedItemIDs.Num() > 0) {
		FHitResult ItemTraceHitResult;
		FVector HitLocation;
		OwningCharacter->TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
		if (ItemTraceHitResult.bBlockingHit) {
			// UE_LOG(LogTemp, Error, TEXT("TraceForQuestsHolders: %s"), *ItemTraceHitResult.Actor->GetName())
			IQuestHolderInterface* Holder = Cast<IQuestHolderInterface>(ItemTraceHitResult.Actor);
			if (Holder) {
				// UE_LOG(LogTemp, Error, TEXT("Holder Actor Setup"))
				QuestActor = Cast<AActor>(ItemTraceHitResult.Actor);
			}

			if (QuestActorLastFrame) {
				if (QuestActorLastFrame != QuestActor) {
					LeaveCachedQuestActorTrace();
				}
			}

			QuestActorLastFrame = QuestActor;
		}
		else if (QuestActorLastFrame) {
			LeaveCachedQuestActorTrace();
		}
	}
	else if (QuestActorLastFrame) {
		LeaveCachedQuestActorTrace();
	}
}


void UQuestSystemComponent::ResetQuest() {
	CurrentQuestID = -1;
	ActiveQuest = FQuest();
}

void UQuestSystemComponent::UpdateCache() {
	for (auto It = QuestCache.CreateConstIterator(); It; ++It) {
		QuestCache.Remove(It.Key());
	}

	if (CurrentQuestID != -1) {
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps;
	}

	for (FQuest Quest : Quests) {
		QuestCache.Add(Quest.Name, Quest);
	}
}

void UQuestSystemComponent::LeaveCachedQuestActorTrace() {
	if (Cast<IQuestHolderInterface>(QuestActorLastFrame)) {
		// Cast<IQuestHolderInterface>(QuestActorLastFrame)->LeaveTrace_Implementation(OwningCharacter,
		// 	OwningCharacter->OverlappedItemIDs);
	}
}

void UQuestSystemComponent::AskForQuest(AActor* InQuestHolderActor) {
	if (auto NPCCharacter = Cast<ANPCQuestCharacter>(InQuestHolderActor)) {
		QuestHolderActor = NPCCharacter;
		UE_LOG(LogTemp, Warning, TEXT("Asking for quest: %s"), *NPCCharacter->GetQuests().Quests[0].Name)
		QuestInfoWidgetStateDelegate.Broadcast(false,
		                                       NPCCharacter->GetQuests().Quests[0].Name,
		                                       NPCCharacter->GetQuests().Quests[0].Description);
	}
}

void UQuestSystemComponent::AcceptQuest() {
	if (QuestHolderActor) {
		if (auto NPCCharacter = Cast<ANPCQuestCharacter>(QuestHolderActor)) {
			AddQuest(NPCCharacter->GetQuests().Quests[0]);
			NPCCharacter->GetQuests().Quests.RemoveAt(0);
		}
	}
}
