// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"

#include "MainCharacter.h"
#include "PhysicsBasedItem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent(): bIsHoldingItem(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
	GrabHandleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GrabHandleComponent"));
	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForItems();
	TraceForLadder();

	HoldCurrentItem();
}

void UItemComponent::TraceForItems() {
	if (OwningCharacter->OverlappedItemIDs.Num() > 0) {
		FHitResult ItemTraceHitResult;
		FVector HitLocation;
		OwningCharacter->TraceUnderCrosshairs(ItemTraceHitResult, HitLocation);
		if (ItemTraceHitResult.bBlockingHit) {
			TraceHitItem = Cast<AItem>(ItemTraceHitResult.GetActor());
			// TraceHitItemHitComponent = ItemTraceHitResult.GetComponent();
			//TODO: If Trace hit item exists, switch between item types...
			if (TraceHitItem) {
				UE_LOG(LogTemp, Warning, TEXT("APhysicsBasedItem::Tracing item: %s"), *TraceHitItem->GetName());
				// TODO: add custom depth...
				if (Cast<APhysicsBasedItem>(TraceHitItem)) {
					TraceHitItemHitComponent = ItemTraceHitResult.GetComponent();
					UE_LOG(LogTemp, Error, TEXT("APhysicsBasedItem::TraceHitItemHitComponent: %s"), *TraceHitItemHitComponent->GetName())
				}
			}

			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterp) {
				TraceHitItem = nullptr;
			}

			if (TraceHitItem && TraceHitItem->GetPickupWidget()) {
				TraceHitItem->PerformTrace(OwningCharacter, OwningCharacter->OverlappedItemIDs);
			}

			if (TraceHitItemLastFrame) {
				if (TraceHitItem != TraceHitItemLastFrame) {
					TraceHitItemLastFrame->LeaveTrace(OwningCharacter, OwningCharacter->OverlappedItemIDs);
				}
			}

			TraceHitItemLastFrame = TraceHitItem;
		}
		else if (TraceHitItemLastFrame) {
			TraceHitItemLastFrame->LeaveTrace(OwningCharacter, OwningCharacter->OverlappedItemIDs);
			TraceHitItemHitComponent = nullptr;
		}
	}
	else if (TraceHitItemLastFrame) {
		TraceHitItemLastFrame->LeaveTrace(OwningCharacter, OwningCharacter->OverlappedItemIDs);
		TraceHitItemHitComponent = nullptr;
	}
}

void UItemComponent::HoldCurrentItem() {
	if (Cast<APhysicsBasedItem>(TraceHitItem) && IsHoldingItem()) {
		FTransform Transform = GrabHandleComponent->GetComponentTransform();
		PhysicsHandleComponent->SetTargetLocationAndRotation(
			Transform.GetLocation(),
			Transform.GetRotation().Rotator()
		);
	}
}

void UItemComponent::TraceForLadder() {
	FHitResult LadderTraceHitResult;
	FVector HitLocation;
	bool Trace = OwningCharacter->TraceForLevelChange(LadderTraceHitResult, HitLocation);
	if (OwningCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb) {
		OwningCharacter->bJumpFromClimb = !Trace;
		OwningCharacter->bTouchingFloor = Trace;
	}
}

bool UItemComponent::IsHoldingItem() {
	return IsValid(PhysicsHandleComponent->GetGrabbedComponent());
}
