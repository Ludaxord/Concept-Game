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
	WallFront = CreateDefaultSubobject<UBoxComponent>(TEXT("WallFront"));

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

	ShelfPosition1->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	ShelfPosition1->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	ShelfPosition2->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	ShelfPosition2->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	WallLeft->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	WallLeft->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	WallRight->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	WallRight->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	WallBack->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	WallBack->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	WallFront->OnComponentBeginOverlap.AddDynamic(this, &ACabinet::OnBoxBeginOverlap);
	WallFront->OnComponentEndOverlap.AddDynamic(this, &ACabinet::OnBoxEndOverlap);

	DoorMovementFunctionFloat.BindDynamic(this, &ACabinet::UpdateDoorMovementTransitionTimeline);
	if (DoorMovementTransitionFloatCurve) {
		DoorMovementTransitionTimeline->AddInterpFloat(DoorMovementTransitionFloatCurve, DoorMovementFunctionFloat);
		CurrentLeftRotYaw = LeftDoorMesh->GetComponentRotation().Yaw;
		CurrentRightRotYaw = RightDoorMesh->GetComponentRotation().Yaw;
		UE_LOG(LogTemp, Warning, TEXT("Setting Door Movement Transition"));
	}


	//TODO: Wrap into a function...
	FCabinetShelf CabinetShelf1 = FCabinetShelf();
	CabinetShelf1.EmptyPlaceAtShelf = ShelfPosition1->Bounds.BoxExtent;
	Shelves.Add(ShelfPosition1, CabinetShelf1);
	// Shelves[ShelfPosition1] = CabinetShelf1;

	FCabinetShelf CabinetShelf2 = FCabinetShelf();
	CabinetShelf2.EmptyPlaceAtShelf = ShelfPosition2->Bounds.BoxExtent;
	Shelves.Add(ShelfPosition2, CabinetShelf2);
	// Shelves[ShelfPosition2] = CabinetShelf2;

	TArray<UBoxComponent*> Keys;
	Shelves.GetKeys(Keys);

	for (AItem* IItem : InsideItems) {
		bool bItemSetup = false;
		for (UBoxComponent* Key : Keys) {
			if (!bItemSetup) {
				FVector Loc = Key->GetComponentLocation();
				FVector MeshBounds = IItem->GetItemMesh()->Bounds.BoxExtent;
				FRotator Rot = Key->GetComponentRotation();

				if (Shelves[Key].ShelfItems.Num() == 0) {
					FVector2D EmptyPlaceAtShelf = {Shelves[Key].EmptyPlaceAtShelf.X, Shelves[Key].EmptyPlaceAtShelf.Y};
					FVector2D ItemBound = EmptyPlaceAtShelf - FVector2D(MeshBounds.X, MeshBounds.Y);
					FVector2D RotatedItemBound = EmptyPlaceAtShelf - FVector2D(MeshBounds.Y, MeshBounds.X);

					//TODO: Set Position And Rotation depend on ItemBox Bounds (if is rotated or base state)
					if (RotatedItemBound > ItemBound && RotatedItemBound.X > 0 && RotatedItemBound.Y > 0) {
						UE_LOG(LogTemp, Error, TEXT("EmptyPlaceAtShelf Set Rotated Item"))
					}


					UE_LOG(LogTemp, Warning,
					       TEXT("EmptyPlaceAtShelf %s, MeshBounds %s, ItemBound %s, RotatedItemBound %s"),
					       *Shelves[Key].EmptyPlaceAtShelf.ToString(),
					       *MeshBounds.ToString(),
					       *ItemBound.ToString(),
					       *RotatedItemBound.ToString())

					Loc = {Loc.X, Loc.Y, Loc.Z + (MeshBounds.Z) + 10};
					Rot = {Rot.Pitch, Rot.Yaw, Rot.Roll};
					IItem->GetItemMesh()->SetupAttachment(Key);
					IItem->GetItemMesh()->SetWorldLocationAndRotation(Loc, Rot);
					IItem->SetItemState(EItemState::EIS_PickupWithPhysics);
					// IItem->SetItemState(EItemState::EIS_Pickup);
					IItem->InteractionEnabled(false);
					IItem->ParentItemReferenceInteractEvent.AddDynamic(this, &ACabinet::ShelfItemsInteraction);

					FShelfItem ShelfItem = FShelfItem();
					ShelfItem.ShelfReference = Key;
					ShelfItem.ShelfItem = IItem;
					ShelfItem.PositionInShelf = FTransform(Rot.Quaternion(), Loc);

					const bool bIsInArray = Shelves[Key].ShelfItems.ContainsByPredicate(
						[&](FShelfItem InShelfItem) -> bool {
							return ShelfItem.ShelfItem == InShelfItem.ShelfItem;
						});

					if (!bIsInArray) {
						UE_LOG(LogTemp, Error, TEXT("Adding Item %s"), *ShelfItem.ShelfItem->GetName())
						Shelves[Key].ShelfItems.Add(ShelfItem);
						bItemSetup = true;
					}
				}
				else if (Shelves[Key].ShelfItems.Num() > 0) {
					// IItem->GetItemMesh()->SetupAttachment(Key);
					// IItem->GetItemMesh()->SetWorldLocationAndRotation(Loc, Rot);
					// IItem->SetItemState(EItemState::EIS_Falling);
					// IItem->SetItemState(EItemState::EIS_Pickup);
					// IItem->InteractionEnabled(false);
				}
			}

		}
	}
}

void ACabinet::InteractWithItem(AMainCharacter* InCharacter) {
	FString CurrentHitItemName = Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Interact With Component ::> %s"), *CurrentHitItemName)

	// if (Character->GetCharacterItemComponent()->GetTraceHitItemHitComponent() == LeftDoorMesh || Character->
	// GetCharacterItemComponent()->GetTraceHitItemHitComponent() == RightDoorMesh) {

	bIsOpenedRef = !bIsOpened;

	TArray<UBoxComponent*> Keys;
	Shelves.GetKeys(Keys);
	for (UBoxComponent* Key : Keys) {
		for (FShelfItem ShelfItem : Shelves[Key].ShelfItems) {
			AItem* IItem = ShelfItem.ShelfItem;
			// for (AItem* IItem : InsideItems) {
			// IItem->SetItemState(bIsOpenedRef ? EItemState::EIS_Pickup : EItemState::EIS_Falling);
			IItem->SetItemState(EItemState::EIS_PickupWithPhysics);
			IItem->InteractionEnabled(bIsOpenedRef);

			WallFront->SetSimulatePhysics(!bIsOpenedRef);
			WallFront->SetCollisionProfileName(bIsOpenedRef ? FName("OverlapAllDynamic") : FName("BlockAll"));

			const TEnumAsByte<EItemState> PoseEnum = IItem->GetItemState();
			FString ItemEnumAsString = UEnum::GetValueAsString(PoseEnum.GetValue());


			FBoxSphereBounds MeshBounds = IItem->GetItemMesh()->Bounds;
			FBoxSphereBounds Shelf1Bounds = ShelfPosition1->Bounds;
			FBoxSphereBounds Shelf2Bounds = ShelfPosition2->Bounds;
			UE_LOG(LogTemp, Warning,
			       TEXT(
				       "ItemBounds Name: %s, \nMeshBounds: %s, \nShelfPosition1: %s, \nShelfPosition2: %s, Shelf1 %s, Slelf2 %s Item State: %s"
			       ),
			       *IItem->GetName(),
			       *MeshBounds.ToString(), *Shelf1Bounds.ToString(), *Shelf2Bounds.ToString(),
			       *(Shelf1Bounds.BoxExtent - MeshBounds.BoxExtent).ToString(),
			       *(Shelf2Bounds.BoxExtent - MeshBounds.BoxExtent).ToString(),
			       *ItemEnumAsString
			)
			// }
		}
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

void ACabinet::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent,
                                 int32 OtherBodyIndex,
                                 bool bFromSweep,
                                 const FHitResult& SweepResult) {
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Error, TEXT("OnBoxOverlap Begin OtherActor: %s OverlappedComponent: %s"), *OtherActor->GetName(),
	       *OverlappedComponent->GetName())
}

void ACabinet::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent,
                               int32 OtherBodyIndex) {
	Super::OnBoxEndOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("OnBoxOverlap End OtherActor: %s OverlappedComponent: %s"), *OtherActor->GetName(),
	       *OverlappedComponent->GetName())
}

void ACabinet::UpdateDoorMovementTransitionTimeline(float Output) {
	CurrentDoorRotation = Output;
}

void ACabinet::ShelfItemsInteraction(AItem* InItem) {
	TArray<UBoxComponent*> Keys;
	Shelves.GetKeys(Keys);
	for (UBoxComponent* Key : Keys) {
		UE_LOG(LogTemp, Display, TEXT("ShelfItemsInteraction Size Before Operation: %i"), Shelves[Key].ShelfItems.Num())

		int32 ItemIndex = Shelves[Key].ShelfItems.IndexOfByPredicate(
			[&](FShelfItem InShelfItem) -> bool {
				return InItem == InShelfItem.ShelfItem;
			});

		if (ItemIndex != INDEX_NONE) {
			Shelves[Key].ShelfItems.RemoveAt(ItemIndex);
		}

		// bool bRemovedFromShelf = Shelves[Key].ShelfItems.FindByPredicate(
		// 	[&](FShelfItem InShelfItem) -> bool {
		// 		bool bRemoved = InItem == InShelfItem.ShelfItem;
		// 		if (bRemoved) {
		// 			Shelves[Key].ShelfItems.Remove(InShelfItem);
		// 		}
		//
		// 		return bRemoved;
		// 	});
		//
		// if (bRemovedFromShelf) {
		// 	InsideItems.Remove(InItem);
		// }

		UE_LOG(LogTemp, Error, TEXT("ShelfItemsInteraction Size After Operation: %i"), Shelves[Key].ShelfItems.Num())
	}
}
