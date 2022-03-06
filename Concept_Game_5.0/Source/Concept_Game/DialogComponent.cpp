// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogComponent.h"

#include "NPCBase.h"
#include "NPCQuestCharacter.h"

UDialogComponent::UDialogComponent() {

}

void UDialogComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());

	AskForDialogDelegate.AddDynamic(this, &UDialogComponent::AskForDialog);
}

void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForDialogHolders();
}


void UDialogComponent::TraceForDialogHolders() {
	if (OwningCharacter->OverlappedItemIDs.Num() > 0) {
		FHitResult ItemTraceHitResult;
		FVector HitLocation;
		OwningCharacter->TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
		if (ItemTraceHitResult.bBlockingHit) {
			// UE_LOG(LogTemp, Error, TEXT("TraceForQuestsHolders: %s"), *ItemTraceHitResult.Actor->GetName())
			IDialogHolderInterface* Holder = Cast<IDialogHolderInterface>(ItemTraceHitResult.GetActor());
			if (Holder) {
				if (Holder->DialogAvailable_Implementation()) {
					// UE_LOG(LogTemp, Error, TEXT("Holder Actor Setup"))
					DialogActor = Cast<AActor>(ItemTraceHitResult.GetActor());
				}
			}

			if (DialogActorLastFrame) {
				if (DialogActorLastFrame != DialogActor) {
					// LeaveCachedDialogActorTrace();
				}
			}

			DialogActorLastFrame = DialogActor;
		}
		else if (DialogActorLastFrame) {
			// LeaveCachedDialogActorTrace();
		}
	}
	else if (DialogActorLastFrame) {
		// LeaveCachedDialogActorTrace();
	}
}

void UDialogComponent::AskForDialog(AActor* InDialogHolderActor) {
	if (auto NPCCharacter = Cast<ANPCQuestCharacter>(InDialogHolderActor)) {
		DialogHolderActor = NPCCharacter;
		AddRemoveDialogWidgetDelegate.Broadcast(!bDialogWidgetVisible);
	}
}
