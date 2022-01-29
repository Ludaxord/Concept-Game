// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestsComponent.h"

#include "QuestHolderInterface.h"
#include "MainCharacter.h"
#include "NPCQuestCharacter.h"


UQuestsComponent::UQuestsComponent() {
}

void UQuestsComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());

	AddRemoveQuestDelegate.AddDynamic(this, &UQuestsComponent::UpdateCache);
	AskForQuestDelegate.AddDynamic(this, &UQuestsComponent::AskForQuest);
	AcceptQuestDelegate.AddDynamic(this, &UQuestsComponent::AcceptQuest);
}

void UQuestsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForQuestsHolders();
}

void UQuestsComponent::AddQuest(FNPCQuest InNPCQuest) {
	FQuest Quest = {InNPCQuest.Name, InNPCQuest.Description, InNPCQuest.QuestSteps, InNPCQuest.bPrimary};
	Quests.Add(Quest);

	if (AddRemoveQuestDelegate.IsBound()) {
		AddRemoveQuestDelegate.Broadcast();

		if (Quest.bPrimary && bCanChangeQuest) {
			SelectQuest(Quest.Name);
		}
	}
}

void UQuestsComponent::SelectQuest(FString InName) {
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

void UQuestsComponent::RemoveQuest() {
	Quests.RemoveAt(CurrentQuestID);
	SelectQuest(FString());
	bCanChangeQuest = true;
}

void UQuestsComponent::CompleteQuestStep() {
	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num() == 0) {
		RemoveQuest();
	}
	else {
		bCanChangeQuest = false;
		UpdateCache();
	}
}

void UQuestsComponent::QuestListToggle() {
	bQuestListVisible = !bQuestListVisible;
	UE_LOG(LogTemp, Warning, TEXT("bQuestList Visible: %s"), bQuestListVisible ? TEXT("true"): TEXT("false"))
}

void UQuestsComponent::TraceForQuestsHolders() {
	UE_LOG(LogTemp, Warning, TEXT("TraceForQuestsHolders TICK"))
	if (OwningCharacter->OverlappedItemIDs.Num() > 0) {
		FHitResult ItemTraceHitResult;
		FVector HitLocation;
		OwningCharacter->TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
		if (ItemTraceHitResult.bBlockingHit) {
			UE_LOG(LogTemp, Error, TEXT("TraceForQuestsHolders: %s"), *ItemTraceHitResult.Actor->GetName())
			IQuestHolderInterface* Holder = Cast<IQuestHolderInterface>(ItemTraceHitResult.Actor);
			if (Holder && Holder->QuestAvailable_Implementation()) {
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


void UQuestsComponent::ResetQuest() {
	CurrentQuestID = -1;
	ActiveQuest = FQuest();
}

void UQuestsComponent::UpdateCache() {
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

void UQuestsComponent::LeaveCachedQuestActorTrace() {
	if (Cast<IQuestHolderInterface>(QuestActorLastFrame)) {
		// Cast<IQuestHolderInterface>(QuestActorLastFrame)->LeaveTrace_Implementation(OwningCharacter,
		// 	OwningCharacter->OverlappedItemIDs);
	}
}

void UQuestsComponent::AskForQuest(AActor* InQuestHolderActor) {
	if (auto NPCCharacter = Cast<ANPCQuestCharacter>(InQuestHolderActor)) {
		QuestHolderActor = NPCCharacter;
		QuestInfoWidgetStateDelegate.Broadcast(false, NPCCharacter->GetQuests().Quests[0].Name,
		                                       NPCCharacter->GetQuests().Quests[0].Description);
	}
}

void UQuestsComponent::AcceptQuest() {
	if (QuestHolderActor) {
		if (auto NPCCharacter = Cast<ANPCQuestCharacter>(QuestHolderActor)) {
			AddQuest(NPCCharacter->GetQuests().Quests[0]);
			NPCCharacter->GetQuests().Quests.RemoveAt(0);
		}
	}
}
