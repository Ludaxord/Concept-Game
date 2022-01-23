// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrigger.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AQuestTrigger::AQuestTrigger() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AQuestTrigger::FinishStep() {
	if (QuestComponentRef) {
		if (QuestComponentRef->ActiveQuest.QuestSteps.Num() > 0) {
			if (QuestComponentRef->ActiveQuest.Name == QuestName &&
				QuestComponentRef->ActiveQuest.QuestSteps[0].Trigger == this) {
				QuestComponentRef->CompleteQuestStep();
				return true;
			}
		}
	}

	return false;
}

// Called when the game starts or when spawned
void AQuestTrigger::BeginPlay() {
	Super::BeginPlay();

	if (const AMainCharacter* OwningCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(this, 0))) {
		QuestComponentRef = OwningCharacter->GetQuestComponent();
	}

}

// Called every frame
void AQuestTrigger::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
