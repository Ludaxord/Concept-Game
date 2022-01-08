// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItem.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

APhysicsBasedItem::APhysicsBasedItem() {
	PhysicsBasedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsBasedMesh"));
	SetRootComponent(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(true);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	CollisionBox->SetupAttachment(PhysicsBasedMesh);

	PickupWidget->SetupAttachment(PhysicsBasedMesh);
	AreaSphere->SetupAttachment(PhysicsBasedMesh);
}

void APhysicsBasedItem::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
	OnPhysicsInteraction();
}

bool APhysicsBasedItem::InteractWith_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent) {
	return IPhysicsBasedInteractionInterface::InteractWith_Implementation(InActor, InHitComponent);
}

AActor* APhysicsBasedItem::LookAt_Implementation(AActor* InActor, UPrimitiveComponent* InHitComponent,
                                                 APlayerController* InController, FText& OutMessage,
                                                 UPrimitiveComponent* OutLookAtComponent) {
	return IPhysicsBasedInteractionInterface::LookAt_Implementation(InActor, InHitComponent, InController, OutMessage,
	                                                                OutLookAtComponent);
}

void APhysicsBasedItem::OnPhysicsInteraction() {
	if (Character) {
		if (bItemCurrentlyOverlapped) {
			if (Character->GetCharacterItemComponent()->IsHoldingItem()) {
				Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->ReleaseComponent();
			}
			else {
				OnLiftItem();
			}
		}
		else {
			Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->ReleaseComponent();
		}
	}
}

void APhysicsBasedItem::OnLiftItem() {
	FHitResult HitResult;
	FVector HitLocation;
	bool bIsHit = Character->TraceUnderCrosshairs(HitResult, HitLocation);
	if (bIsHit) {
		Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->GrabComponentAtLocationWithRotation(
			Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent(),
			FName("None"),
			HitResult.Location,
			UKismetMathLibrary::MakeRotFromX(HitResult.Normal)
		);
		Character->GetCharacterItemComponent()->GetGrabHandleComponent()->SetWorldLocationAndRotation(
			HitResult.Location,
			UKismetMathLibrary::MakeRotFromX(HitResult.Normal));
	}
}

void APhysicsBasedItem::OnDropItem() {
}

void APhysicsBasedItem::OnThrowItem() {
}
