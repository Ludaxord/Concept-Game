// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemComponent.h"

#include "MainCharacter.h"
#include "NPCQuestCharacter.h"
#include "QuestHolderInterface.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent(): bCanChangeQuest(true) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSystemComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());

	// AddRemoveQuestDelegate.AddDynamic(this, &UQuestSystemComponent::UpdateCache);
	AskForQuestDelegate.AddDynamic(this, &UQuestSystemComponent::AskForQuest);
	AcceptQuestDelegate.AddDynamic(this, &UQuestSystemComponent::AcceptQuest);

	TimerDelegate.BindLambda([&] {
		UE_LOG(LogTemp, Warning, TEXT("Delayed Step Update... CurrentQuestID: %i"), CurrentQuestID)
		StepUpdateDelegate.Broadcast(true, CurrentQuestID != -1);
	});

	ResetQuest();
}

void UQuestSystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UpdateCache();
}


// Called every frame
void UQuestSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForQuestsHolders();
}

void UQuestSystemComponent::AddQuest(FNPCQuest InNPCQuest) {
	UE_LOG(LogTemp, Warning, TEXT("Adding Quest...."))
	FQuest Quest = {InNPCQuest.Name, InNPCQuest.Description, InNPCQuest.QuestSteps, InNPCQuest.bPrimary};
	Quests.Add(Quest);

	AddRemoveQuestDelegate.Broadcast();

	if (Quest.bPrimary && bCanChangeQuest) {
		SelectQuest(Quest.Name);
	}
}

void UQuestSystemComponent::SelectQuest(FString InName) {
	UE_LOG(LogTemp, Warning, TEXT("Selecting Quest: %s"), *InName)
	UE_LOG(LogTemp, Warning, TEXT("Quests Num: %i"), Quests.Num())
	if (Quests.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Active Quest: %s"), *ActiveQuest.Name)
		if (ActiveQuest.Name != InName) {
			ResetQuest();
			if (ActiveQuest.Name == "") {
				CurrentQuestID = 0;
				ActiveQuest = Quests[CurrentQuestID];

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, .7f, false);
			}
			else {
				for (int i = 0; i < Quests.Num(); i++) {
					if (Quests.IsValidIndex(i)) {
						if (Quests[i].Name == InName) {
							CurrentQuestID = i;
							ActiveQuest = Quests[i];
							break;
						}
					}
				}

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, .6f, false);
			}
		}
	}
	else {
		ResetQuest();
	}

	UE_LOG(LogTemp, Error, TEXT("CurrentQuestID: %i"), CurrentQuestID)
}

void UQuestSystemComponent::RemoveQuest(FString InName) {
	UE_LOG(LogTemp, Warning, TEXT("Removing Quest: %i"), CurrentQuestID)
	QuestNotificationDelegate.Broadcast(true, InName);
	Quests.RemoveAt(CurrentQuestID);
	SelectQuest(FString());
	bCanChangeQuest = true;
	AddRemoveQuestDelegate.Broadcast();
}

void UQuestSystemComponent::CompleteQuestStep() {
	UE_LOG(LogTemp, Warning, TEXT("CompleteQuestStep..."))
	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num() == 0) {
		RemoveQuest(ActiveQuest.Name);
	}
	else {
		bCanChangeQuest = false;
		UpdateCache();
		UE_LOG(LogTemp, Error, TEXT("Complete quest Step CurrentQuestID: %i"), CurrentQuestID)
		StepUpdateDelegate.Broadcast(true, CurrentQuestID != -1);
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
	UE_LOG(LogTemp, Warning, TEXT("RESET QUEST CurrentQuestID: %i"), CurrentQuestID)
	StepUpdateDelegate.Broadcast(true, CurrentQuestID != -1);
}

void UQuestSystemComponent::UpdateCache() {
	UE_LOG(LogTemp, Warning, TEXT("Updating Cache...."))
	for (auto It = QuestCache.CreateConstIterator(); It; ++It) {
		QuestCache.Remove(It.Key());
	}

	if (CurrentQuestID != -1) {
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps;
	}

	for (FQuest Quest : Quests) {
		UE_LOG(LogTemp, Warning, TEXT("Adding Quest: %s"), *Quest.Name)
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
	UE_LOG(LogTemp, Warning, TEXT("AcceptQuest : Exists %s"),
	       QuestHolderActor != nullptr ? TEXT("true") : TEXT("false"))
	if (QuestHolderActor) {
		if (ANPCQuestCharacter* NPCCharacter = Cast<ANPCQuestCharacter>(QuestHolderActor)) {
			if (NPCCharacter->GetQuests().Quests.IsValidIndex(0)) {

				FNPCQuest Quest = NPCCharacter->GetQuests().Quests[0];
				AddQuest(Quest);

				//TODO: Fix problem with removing struct from array, now it works from blueprints...
				// NPCCharacter->GetQuests().Quests.RemoveAt(0);
				// UE_LOG(LogTemp, Warning, TEXT("Current NPC Quests: %i"), NPCCharacter->GetQuests().Quests.Num());
				// NPCCharacter->GetQuests().Quests.RemoveAt(0);
				// NPCCharacter->GetQuests().Quests.Empty();
				//
				// UE_LOG(LogTemp, Warning, TEXT("Current NPC Quests After Remove: %i"),
				//        NPCCharacter->GetQuests().Quests.Num());
				// for (FNPCQuest Q : NPCCharacter->GetQuests().Quests) {
				// 	UE_LOG(LogTemp, Warning, TEXT("Quests: %s"), Q == Quest ? TEXT("true") : TEXT("false"))
				// }
			}
		}
	}
}
