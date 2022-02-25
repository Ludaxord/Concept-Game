// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItem.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Launch/Resources/Version.h"


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
	UE_LOG(LogTemp, Warning, TEXT("APhysicsBasedItem::OnPhysicsInteraction"))
	if (Character) {
		if (bItemCurrentlyOverlapped) {
			if (Character->GetCharacterItemComponent()->IsHoldingItem()) {
				OnDropItem();
			}
			else {
				OnLiftItem();
			}
		}
		else {
			OnDropItem();
		}
	}
}

void APhysicsBasedItem::OnLiftItem() {
	UE_LOG(LogTemp, Warning, TEXT("APhysicsBasedItem::OnLiftItem"))
	FHitResult HitResult;
	FVector HitLocation;
	if (Character->TraceUnderCrosshairs(HitResult, HitLocation)) {
#if ENGINE_MAJOR_VERSION == 5
		Character->GetCharacterItemComponent()->GetPhysicsConstraintComponent()->SetConstrainedComponents(
			Character->GetMesh(), FName("hand_r"), HitResult.GetComponent(), HitResult.BoneName);
		HitResult.GetComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		                                                        ECollisionResponse::ECR_Ignore);
		ItemHolder = HitResult.GetComponent();
#else
		UE_LOG(LogTemp, Warning, TEXT("APhysicsBasedItem::TraceHitItemHitComponent: %s"),
		       *Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->GetName())
		Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->GrabComponentAtLocationWithRotation(
			Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent(),
			FName("None"),
			HitResult.Location,
			UKismetMathLibrary::MakeRotFromX(HitResult.Normal)
		);
		Character->GetCharacterItemComponent()->GetGrabHandleComponent()->SetWorldLocationAndRotation(
			HitResult.Location,
			UKismetMathLibrary::MakeRotFromX(HitResult.Normal));
#endif
	}
}

void APhysicsBasedItem::OnDropItem() {
#if ENGINE_MAJOR_VERSION == 5

#else
	Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->ReleaseComponent();
#endif
}

void APhysicsBasedItem::OnThrowItem() {
#if ENGINE_MAJOR_VERSION == 5

#else

#endif
}
