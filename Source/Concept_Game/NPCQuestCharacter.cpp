// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCQuestCharacter.h"

#include "QuestTrigger.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

ANPCQuestCharacter::ANPCQuestCharacter() {
	QuestIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIndicatorWidget"));
	QuestIndicatorWidget->SetupAttachment(GetRootComponent());
}

void ANPCQuestCharacter::BeginPlay() {
	Super::BeginPlay();
	LoadQuestsToNPC();
}

void ANPCQuestCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ANPCQuestCharacter::Interact_Implementation() {
	Super::Interact_Implementation();
}

void ANPCQuestCharacter::LoadQuestsToNPC() {
	for (FNPCQuest NPCQuest : Quests.Quests) {
		for (FQuestStep QuestStep : NPCQuest.QuestSteps) {
			if (QuestStep.Trigger) {
				QuestStep.Trigger->QuestName = NPCQuest.Name;
				QuestStep.Trigger->QuestStep = QuestStep;
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Trigger Is not Valid"))
			}
		}
	}
}
