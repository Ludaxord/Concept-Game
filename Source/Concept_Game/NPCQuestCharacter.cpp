// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCQuestCharacter.h"

#include "QuestTrigger.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

ANPCQuestCharacter::ANPCQuestCharacter() {
	QuestIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIndicatorWidget"));
	QuestIndicatorWidget->SetupAttachment(GetRootComponent());

	NPCSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPCQuestCharacter::OnSphereBeginOverlap_Implementation);
	NPCSphere->OnComponentEndOverlap.AddDynamic(this, &ANPCQuestCharacter::OnSphereEndOverlap_Implementation);
}

void ANPCQuestCharacter::BeginPlay() {
	Super::BeginPlay();

	LoadQuestsToNPC();
}

void ANPCQuestCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ANPCQuestCharacter::Interact_Implementation(AMainCharacter* InCharacter) {
	Super::Interact_Implementation(InCharacter);

	if (Quests.Quests.Num() > 0) {
		if (Quests.Quests[0].bCanBeAccepted) {
			InCharacter->GetQuestComponent()->AskForQuestDelegate.Broadcast(this);
		}
	}
}

void ANPCQuestCharacter::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                             AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                                                             int32 OtherBodyIndex, bool bFromSweep,
                                                             const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Overlapping Begin Quest %s Overlapped Component %s"), *GetName(),
			       *OverlappedComponent->GetName());
			OtherCharacter->SphereOverlapBegin(ID);
		}
	}

}

void ANPCQuestCharacter::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Overlapping End Quest %s Overlapped Component %s"), * GetName(),
			       *OverlappedComponent->GetName());
			OtherCharacter->SphereOverlapEnd(ID);
		}
	}
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
