// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsBasedItem.h"

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Launch/Resources/Version.h"


APhysicsBasedItem::APhysicsBasedItem(): ImpulsePower(100000.f) {
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
#if ENGINE_MAJOR_VERSION == 5
		if (ItemHolder != nullptr) {
			OnDropItem();
		}
		else {
			OnLiftItem();
		}
#else
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
#endif
	}
}

void APhysicsBasedItem::OnLiftItem() {
	UE_LOG(LogTemp, Warning, TEXT("APhysicsBasedItem::OnLiftItem"))
	FHitResult HitResult;
	FVector HitLocation;
	if (Character->TraceUnderCrosshairs(HitResult, HitLocation)) {
		InteractHitResult = HitResult;
#if ENGINE_MAJOR_VERSION == 5
		Character->GetItemHoldMeshComponent()->SetWorldLocation(HitResult.ImpactPoint);
		Character->GetCharacterItemComponent()->GetPhysicsConstraintComponent()->SetConstrainedComponents(
			Character->GetItemHoldMeshComponent(),
			FName("None"),
			HitResult.GetComponent(),
			HitResult.BoneName);
		// SetItemState(EItemState::EIS_Equipped);
		HitResult.GetComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		                                                        ECollisionResponse::ECR_Ignore);
		ItemHolder = HitResult.GetComponent();

		if (!Character->GetCharacterItemComponent()->GetGrabInPlace()) {
			FLatentActionInfo Info = FLatentActionInfo();
			Info.CallbackTarget = this;
			Info.ExecutionFunction = "MoveItem";
			Info.Linkage = 0;
			Info.UUID = FMath::Rand();

			UKismetSystemLibrary::MoveComponentTo(Character->GetItemHoldMeshComponent(),
			                                      Character->GetItemHoldPlacementComponent()->GetRelativeLocation(),
			                                      Character->GetItemHoldMeshComponent()->GetRelativeRotation(),
			                                      true,
			                                      true,
			                                      0.3,
			                                      false,
			                                      EMoveComponentAction::Type::Move,
			                                      Info);
		}


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
	Character->GetCharacterItemComponent()->GetPhysicsConstraintComponent()->BreakConstraint();
	ItemHolder->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	// SetItemState(EItemState::EIS_Pickup);
	ItemHolder = nullptr;
	InteractHitResult = {};
#else
	Character->GetCharacterItemComponent()->GetPhysicsHandleComponent()->ReleaseComponent();
#endif
}

void APhysicsBasedItem::OnThrowItem() {
#if ENGINE_MAJOR_VERSION == 5
	if (ItemHolder != nullptr) {
		FHitResult HitResult;
		FVector HitLocation;
		if (Character->TraceUnderCrosshairs(HitResult, HitLocation)) {
			OnDropItem();
			HitResult.GetComponent()->AddImpulseAtLocation(
				Character->GetEyesCamera()->GetForwardVector() * ImpulsePower,
				HitResult.ImpactPoint,
				HitResult.BoneName);
		}
	}
#else

#endif
}
