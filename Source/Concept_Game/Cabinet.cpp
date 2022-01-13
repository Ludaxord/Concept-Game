// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabinet.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACabinet::ACabinet(): bIsOpened(false) {
	ItemInteractionName = "Open";
	ItemName = "Cabinet";

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));

	LeftDoorMesh->SetupAttachment(PhysicsBasedMesh);
	RightDoorMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorMesh->SetSimulatePhysics(true);
	RightDoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	LeftDoorPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftDoorPhysicsConstraint"));
	RightDoorPhysicsConstraint = CreateDefaultSubobject<
		UPhysicsConstraintComponent>(TEXT("RightDoorPhysicsConstraint"));

	DoorMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorMovementTransitionTimeline"));

	ShelfPosition1 = CreateDefaultSubobject<UBoxComponent>(TEXT("ShelfPosition1"));
	ShelfPosition2 = CreateDefaultSubobject<UBoxComponent>(TEXT("ShelfPosition2"));

	WallLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("WallLeft"));
	WallRight = CreateDefaultSubobject<UBoxComponent>(TEXT("WallRight"));
	WallBack = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBack"));

	ShelfPosition1->SetupAttachment(PhysicsBasedMesh);
	ShelfPosition2->SetupAttachment(PhysicsBasedMesh);

	// Item1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item1"));
	// Item1->SetupAttachment(PhysicsBasedMesh);
	// Item2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item2"));
	// Item2->SetupAttachment(PhysicsBasedMesh);
	// Item3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item3"));
	// Item3->SetupAttachment(PhysicsBasedMesh);
	// Item4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item4"));
	// Item4->SetupAttachment(PhysicsBasedMesh);
	// Item5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item5"));
	// Item5->SetupAttachment(PhysicsBasedMesh);

	// LeftDoorPhysicsConstraint->SetupAttachment(LeftDoorMesh);
	// RightDoorPhysicsConstraint->SetupAttachment(RightDoorMesh);

	// LeftDoorPhysicsConstraint->ComponentName1 = {"LeftDoorMesh"};
	// LeftDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};

	// RightDoorPhysicsConstraint->ComponentName1 = {"RightDoorMesh"};
	// RightDoorPhysicsConstraint->ComponentName2 = {"PhysicsBasedMesh"};
}

void ACabinet::BeginPlay() {
	Super::BeginPlay();

	DoorMovementFunctionFloat.BindDynamic(this, &ACabinet::UpdateDoorMovementTransitionTimeline);
	if (DoorMovementTransitionFloatCurve) {
		DoorMovementTransitionTimeline->AddInterpFloat(DoorMovementTransitionFloatCurve, DoorMovementFunctionFloat);
		CurrentLeftRotYaw = LeftDoorMesh->GetComponentRotation().Yaw;
		CurrentRightRotYaw = RightDoorMesh->GetComponentRotation().Yaw;
		UE_LOG(LogTemp, Warning, TEXT("Setting Door Movement Transition"));
	}

	for (AItem* IItem : InsideItems) {
		FVector Loc = ShelfPosition1->GetComponentLocation();
		FBoxSphereBounds MeshBounds = IItem->GetItemMesh()->Bounds;
		Loc = {Loc.X, Loc.Y, Loc.Z + (MeshBounds.BoxExtent.Z) + 10};
		FRotator Rot = ShelfPosition1->GetComponentRotation();
		Rot = {Rot.Pitch + 90, Rot.Yaw, Rot.Roll + 90};
		IItem->GetItemMesh()->SetupAttachment(ShelfPosition1);
		IItem->GetItemMesh()->SetWorldLocationAndRotation(Loc, Rot);
		IItem->SetItemState(EItemState::EIS_Falling);
		// IItem->SetItemState(EItemState::EIS_Pickup);
		IItem->InteractionEnabled(false);
	}
}

void ACabinet::InteractWithItem(AMainCharacter* InCharacter) {
	FString CurrentHitItemName = Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Interact With Component ::> %s"), *CurrentHitItemName)

	// if (Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent() == LeftDoorMesh || Character->
	// GetCharacterItemComponent()->GetTraceHitItemHitComponent() == RightDoorMesh) {

	bIsOpenedRef = !bIsOpened;
	for (AItem* IItem : InsideItems) {
		IItem->SetItemState(bIsOpenedRef ? EItemState::EIS_Pickup : EItemState::EIS_Falling);
		IItem->InteractionEnabled(bIsOpenedRef);

		FBoxSphereBounds MeshBounds = IItem->GetItemMesh()->Bounds;
		FBoxSphereBounds Shelf1Bounds = ShelfPosition1->Bounds;
		FBoxSphereBounds Shelf2Bounds = ShelfPosition2->Bounds;
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "ItemBounds Name: %s, \nMeshBounds: %s, \nShelfPosition1: %s, \nShelfPosition2: %s, Shelf1 %s, Slelf2 %s"
		       ),
		       *IItem->GetName(),
		       *MeshBounds.ToString(), *Shelf1Bounds.ToString(), *Shelf2Bounds.ToString(),
		       *(Shelf1Bounds.BoxExtent - MeshBounds.BoxExtent).ToString(),
		       *(Shelf2Bounds.BoxExtent - MeshBounds.BoxExtent).ToString())
	}

	DoorMovementTransitionTimeline->PlayFromStart();
	// }
	// else {
	// Super::InteractWithItem(InCharacter);
	// }
}

void ACabinet::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bIsOpenedRef != bIsOpened) {
		FRotator LRot = LeftDoorMesh->GetComponentRotation();
		FRotator RRot = RightDoorMesh->GetComponentRotation();

		float LeftLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                ? CurrentLeftRotYaw
			                                                : CurrentLeftRotYaw - 110,
		                                                bIsOpenedRef
			                                                ? CurrentLeftRotYaw - 110
			                                                : CurrentLeftRotYaw,
		                                                CurrentDoorRotation);
		float RightLerpRotYaw = UKismetMathLibrary::Lerp(bIsOpenedRef
			                                                 ? CurrentLeftRotYaw
			                                                 : CurrentLeftRotYaw + 110,
		                                                 bIsOpenedRef
			                                                 ? CurrentLeftRotYaw + 110
			                                                 : CurrentLeftRotYaw,
		                                                 CurrentDoorRotation);

		FRotator LNewRot = {LRot.Pitch, LeftLerpRotYaw, LRot.Roll};
		FRotator RNewRot = {LRot.Pitch, RightLerpRotYaw, LRot.Roll};

		LeftDoorMesh->SetWorldLocationAndRotation(LeftDoorMesh->GetComponentLocation(), LNewRot);
		RightDoorMesh->SetWorldLocationAndRotation(RightDoorMesh->GetComponentLocation(), RNewRot);
		if (CurrentDoorRotation == 1.f) {
			bIsOpened = !bIsOpened;
			CurrentDoorRotation = 0;
		}
	}
}

void ACabinet::UpdateDoorMovementTransitionTimeline(float Output) {
	CurrentDoorRotation = Output;
}
