// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrigger.h"

#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
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

	ID = FGuid::NewGuid();

	if (const AMainCharacter* OwningCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(this, 0))) {
		QuestComponentRef = OwningCharacter->GetQuestComponent();
	}

}

// Called every frame
void AQuestTrigger::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AQuestTrigger::QuestInteract_Implementation(AMainCharacter* InCharacter) {

}

bool AQuestTrigger::QuestAvailable_Implementation() {
	//TODO: check if quest can step is available, for not set it to true
	return true;
}

void AQuestTrigger::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Overlapping Begin Quest %s Overlapped Component %s"), *GetName(),
			       *OverlappedComponent->GetName());
			OtherCharacter->SphereOverlapBegin(ID);
		}
	}
}

void AQuestTrigger::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
