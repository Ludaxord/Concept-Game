// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestsComponent.h"


UQuestsComponent::UQuestsComponent() {
}

void UQuestsComponent::BeginPlay() {
	Super::BeginPlay();
	AddRemoveQuestDelegate.AddDynamic(this, &UQuestsComponent::UpdateCache);
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