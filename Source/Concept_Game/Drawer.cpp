// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADrawer::ADrawer(): bMoveDrawer(false), CurrentDrawerIndex(INDEX_NONE) {
	ItemInteractionName = "Open";
	ItemName = "Drawer";

	TopDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopDrawerMesh"));
	BottomDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomDrawerMesh"));
	MiddleDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiddleDrawerMesh"));

	TopDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	BottomDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	MiddleDrawerMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	TopDrawerMesh->SetSimulatePhysics(true);
	TopDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	BottomDrawerMesh->SetSimulatePhysics(true);
	BottomDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	MiddleDrawerMesh->SetSimulatePhysics(true);
	MiddleDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	ArrowForwardComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowForwardComponent"));
	ArrowForwardComponent->SetupAttachment(PhysicsBasedMesh);

	DrawerMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(
		TEXT("DrawerMovementTransitionTimeline"));


	TopDrawerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("TopDrawerPosition"));
	TopDrawerLeftPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("TopDrawerLeftPosition"));
	TopDrawerRightPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("TopDrawerRightPosition"));
	TopDrawerBackPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("TopDrawerBackPosition"));
	TopDrawerFrontPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("TopDrawerFrontPosition"));

	BottomDrawerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomDrawerPosition"));
	BottomDrawerLeftPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomDrawerLeftPosition"));
	BottomDrawerRightPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomDrawerRightPosition"));
	BottomDrawerBackPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomDrawerBackPosition"));
	BottomDrawerFrontPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomDrawerFrontPosition"));

	MiddleDrawerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("MiddleDrawerPosition"));
	MiddleDrawerLeftPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("MiddleDrawerLeftPosition"));
	MiddleDrawerRightPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("MiddleDrawerRightPosition"));
	MiddleDrawerBackPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("MiddleDrawerBackPosition"));
	MiddleDrawerFrontPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("MiddleDrawerFrontPosition"));

	TopDrawerPosition->SetupAttachment(TopDrawerMesh);
	TopDrawerLeftPosition->SetupAttachment(TopDrawerMesh);
	TopDrawerRightPosition->SetupAttachment(TopDrawerMesh);
	TopDrawerBackPosition->SetupAttachment(TopDrawerMesh);
	TopDrawerFrontPosition->SetupAttachment(TopDrawerMesh);

	BottomDrawerPosition->SetupAttachment(BottomDrawerMesh);
	BottomDrawerLeftPosition->SetupAttachment(BottomDrawerMesh);
	BottomDrawerRightPosition->SetupAttachment(BottomDrawerMesh);
	BottomDrawerBackPosition->SetupAttachment(BottomDrawerMesh);
	BottomDrawerFrontPosition->SetupAttachment(BottomDrawerMesh);

	MiddleDrawerPosition->SetupAttachment(MiddleDrawerMesh);
	MiddleDrawerLeftPosition->SetupAttachment(MiddleDrawerMesh);
	MiddleDrawerRightPosition->SetupAttachment(MiddleDrawerMesh);
	MiddleDrawerBackPosition->SetupAttachment(MiddleDrawerMesh);
	MiddleDrawerFrontPosition->SetupAttachment(MiddleDrawerMesh);
}

void ADrawer::BeginPlay() {
	Super::BeginPlay();

	Character = Cast<AMainCharacter>(GetOwner());

	DrawerMeshes.Add(BottomDrawerPosition, BottomDrawerMesh);
	DrawerMeshes.Add(MiddleDrawerPosition, MiddleDrawerMesh);
	DrawerMeshes.Add(TopDrawerPosition, TopDrawerMesh);

	TArray<UBoxComponent*> Keys;
	DrawerMeshes.GetKeys(Keys);

	struct TempItem {
		AItem* TItem;
		bool bSetup = false;
	};
	TArray<TempItem> TempItems;

	for (AItem* IItem : InsideItems) {
		TempItems.Add(TempItem{IItem});
	}


	for (UBoxComponent* DrawerBoxMesh : Keys) {

		int ItemSetIndex = -1;
		bool bSetupInDrawer = false;

		FDrawerElement DrawerElement = FDrawerElement();

		DrawerElement.bIsOpened = false;
		DrawerElement.DrawerMesh = DrawerMeshes[DrawerBoxMesh];
		DrawerElement.DrawerBoxMesh = DrawerBoxMesh;
		DrawerElement.DrawerLoc = DrawerMeshes[DrawerBoxMesh]->GetComponentLocation();
		DrawerElement.DrawerRelativeLoc = DrawerMeshes[DrawerBoxMesh]->GetRelativeLocation();


		for (int i = 0; i < TempItems.Num(); i++) {
			TempItem TItem = TempItems[i];
			AItem* IItem = TItem.TItem;
			// UE_LOG(LogTemp, Warning, TEXT("Drawer: Item %s Is Setup: %s bSetupInDrawer: %s"), *IItem->GetName(),
			//        TItem.bSetup ? TEXT("True") : TEXT("false"), bSetupInDrawer ? TEXT("true") : TEXT("false"))
			if (!TItem.bSetup && !bSetupInDrawer) {
				FVector Loc = DrawerElement.DrawerBoxMesh->GetRelativeLocation();
				FVector MeshBounds = IItem->GetItemMesh()->Bounds.BoxExtent;
				FRotator Rot = DrawerElement.DrawerBoxMesh->GetRelativeRotation();

				if (DrawerElement.DrawerItems.Num() == 0) {
					FVector2D EmptyPlaceAtShelf = {
						DrawerElement.EmptyPlaceAtDrawer.X, DrawerElement.EmptyPlaceAtDrawer.Y
					};
					FVector2D ItemBound = EmptyPlaceAtShelf - FVector2D(MeshBounds.X, MeshBounds.Y);
					FVector2D RotatedItemBound = EmptyPlaceAtShelf - FVector2D(MeshBounds.Y, MeshBounds.X);

					//TODO: Set Position And Rotation depend on ItemBox Bounds (if is rotated or base state)
					if (RotatedItemBound > ItemBound && RotatedItemBound.X > 0 && RotatedItemBound.Y > 0) {
						UE_LOG(LogTemp, Error, TEXT("EmptyPlaceAtShelf Set Rotated Item"))
					}


					UE_LOG(LogTemp, Warning,
					       TEXT("EmptyPlaceAtShelf %s, MeshBounds %s, ItemBound %s, RotatedItemBound %s"),
					       *DrawerElement.EmptyPlaceAtDrawer.ToString(),
					       *MeshBounds.ToString(),
					       *ItemBound.ToString(),
					       *RotatedItemBound.ToString())

					//TODO: Change na relative location and rotation
					Loc = {Loc.X, Loc.Y, Loc.Z + (MeshBounds.Y)};
					Rot = {Rot.Pitch + 90, Rot.Yaw, Rot.Roll};
					// IItem->GetItemMesh()->SetWorldLocationAndRotation(Loc, Rot);

					IItem->GetItemMesh()->SetupAttachment(DrawerElement.DrawerBoxMesh);
					// IItem->SetItemState(EItemState::EIS_PickupWithPhysics);
					IItem->SetItemState(EItemState::EIS_Pickup);
					IItem->InteractionEnabled(false);
					if (!IItem->ParentItemReferenceInteractEvent.IsBound()) {
						IItem->ParentItemReferenceInteractEvent.AddDynamic(this, &ADrawer::DrawerItemInteraction);
					}

					FDrawerItem DrawerItem = FDrawerItem();
					DrawerItem.DrawerReference = DrawerBoxMesh;
					DrawerItem.Item = IItem;
					DrawerItem.PositionInDrawer = FTransform(Rot.Quaternion(), Loc);


					const bool bIsInArray = DrawerElement.DrawerItems.ContainsByPredicate(
						[&](FDrawerItem InShelfItem) -> bool {
							return DrawerItem.Item == InShelfItem.Item;
						});

					if (!bIsInArray && !TItem.bSetup) {
						DrawerElement.DrawerItems.Add(DrawerItem);
						TItem.bSetup = true;
						bSetupInDrawer = true;
						ItemSetIndex = i;
						UE_LOG(LogTemp, Error, TEXT("Adding Drawer Item %s To Drawer: %s"),
						       *DrawerItem.Item->GetName(), *DrawerElement.DrawerMesh->GetName())
					}

				}
				else if (DrawerElement.DrawerItems.Num() > 0) {
					// IItem->GetItemMesh()->SetupAttachment(Key);
					// IItem->GetItemMesh()->SetWorldLocationAndRotation(Loc, Rot);
					// IItem->SetItemState(EItemState::EIS_Falling);
					// IItem->SetItemState(EItemState::EIS_Pickup);
					// IItem->InteractionEnabled(false);
				}
			}
		}

		if (ItemSetIndex != -1) {
			TempItems.RemoveAt(ItemSetIndex);
		}
		Drawers.Add(DrawerElement);
	}

	DrawerMovementFunctionFloat.BindDynamic(this, &ADrawer::UpdateDrawerMovementTransitionTimeline);
	if (DrawerMovementTransitionFloatCurve) {
		DrawerMovementTransitionTimeline->AddInterpFloat(DrawerMovementTransitionFloatCurve,
		                                                 DrawerMovementFunctionFloat);
	}
}

void ADrawer::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	FindTracingComponent();
	UpdateDrawerMovement();
}


void ADrawer::FindTracingComponent() {
	FHitResult HitResult;
	FVector Loc;
	if (bItemCurrentlyOverlapped) {
		if (Character) {
			if (Character->TraceUnderCrosshairs(HitResult, Loc)) {
				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (UStaticMeshComponent* StaticMeshHitComponent = Cast<UStaticMeshComponent>(HitComponent)) {

					TArray<UBoxComponent*> Keys;
					DrawerMeshes.GetKeys(Keys);
					for (UBoxComponent* BoxMesh : Keys) {
						if (DrawerMeshes[BoxMesh] == StaticMeshHitComponent) {
							CurrentTracingDrawerMesh = StaticMeshHitComponent;
						}
					}
				}
				else {
					CurrentTracingDrawerMesh = nullptr;
				}
			}
			else {
				CurrentTracingDrawerMesh = nullptr;
			}
		}
		else {
			CurrentTracingDrawerMesh = nullptr;
		}
	}
	else {
		CurrentTracingDrawerMesh = nullptr;
	}
}

void ADrawer::UpdateDrawerMovement() {
	if (bMoveDrawer) {
		if (CurrentDrawerIndex != INDEX_NONE) {
			if (CurrentInteractingDrawerMesh != nullptr) {

				float NewRelativeLoc = UKismetMathLibrary::Lerp(Drawers[CurrentDrawerIndex].bIsOpened
					                                                ? Drawers[CurrentDrawerIndex].DrawerRelativeLoc.Y
					                                                : Drawers[CurrentDrawerIndex].DrawerRelativeLoc.Y +
					                                                25.f,
				                                                Drawers[CurrentDrawerIndex].bIsOpened
					                                                ? Drawers[CurrentDrawerIndex].DrawerRelativeLoc.Y +
					                                                25.f
					                                                : Drawers[CurrentDrawerIndex].DrawerRelativeLoc.Y,
				                                                CurrentDrawerMovement);

				CurrentInteractingDrawerMesh->SetRelativeLocationAndRotation(
					{
						CurrentInteractingDrawerMesh->GetRelativeLocation().X,
						NewRelativeLoc,
						CurrentInteractingDrawerMesh->GetRelativeLocation().Z
					}
					,
					CurrentInteractingDrawerMesh->GetRelativeRotation());

				// UE_LOG(LogTemp, Warning, TEXT("Initial RelativeLoc: %s New RelativeLoc: %s Is Opened: %s"),
				//        *Drawers[CurrentDrawerIndex].DrawerRelativeLoc.ToString(),
				//        * CurrentInteractingDrawerMesh->GetRelativeLocation().ToString(),
				//        Drawers[CurrentDrawerIndex].bIsOpened ? TEXT("true") : TEXT("false"))
			}

		}

		if (CurrentDrawerMovement == 1.f) {
			bMoveDrawer = false;
			CurrentDrawerMovement = 0;
			CurrentDrawerIndex = INDEX_NONE;
			CurrentInteractingDrawerMesh = nullptr;
		}
	}
}

void ADrawer::InteractWithItem(AMainCharacter* InCharacter) {
	if (Character && CurrentTracingDrawerMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Interact Component: %s"),
		       *CurrentTracingDrawerMesh->GetName())
		CurrentDrawerIndex = Drawers.IndexOfByPredicate(
			[&](FDrawerElement InDrawerElement) -> bool {
				return CurrentTracingDrawerMesh == InDrawerElement.DrawerMesh;
			});

		if (CurrentDrawerIndex != INDEX_NONE) {
			CurrentInteractingDrawerMesh = CurrentTracingDrawerMesh;
			Drawers[CurrentDrawerIndex].bIsOpened = !Drawers[CurrentDrawerIndex].bIsOpened;
			ItemInteractionName = Drawers[CurrentDrawerIndex].bIsOpened ? "Close" : "Open";

			for (const FDrawerItem ShelfItem : Drawers[CurrentDrawerIndex].DrawerItems) {
				AItem* IItem = ShelfItem.Item;
				FVector Loc = Drawers[CurrentDrawerIndex].DrawerBoxMesh->GetRelativeLocation();
				IItem->SetActorRelativeLocation({Loc.X, Loc.Y + 25.f, Loc.Z});
				// IItem->SetItemState(EItemState::EIS_PickupWithPhysics);
				IItem->SetItemState(EItemState::EIS_Pickup);
				IItem->InteractionEnabled(Drawers[CurrentDrawerIndex].bIsOpened);
				UE_LOG(LogTemp, Error, TEXT("Drawer: %s Loc: %s FDrawerItem => %s"),
				       *Drawers[CurrentDrawerIndex].DrawerMesh->GetName(), *Loc.ToString(), *IItem->GetName())
			}

			bMoveDrawer = true;
			DrawerMovementTransitionTimeline->PlayFromStart();
		}
	}
}

void ADrawer::UpdateDrawerMovementTransitionTimeline(float Output) {
	CurrentDrawerMovement = Output;
}

void ADrawer::DrawerItemInteraction(AItem* InItem) {
	for (FDrawerElement Key : Drawers) {
		int32 ItemIndex = Key.DrawerItems.IndexOfByPredicate(
			[&](FDrawerItem InDrawerItem) -> bool {
				return InItem == InDrawerItem.Item;
			});

		if (ItemIndex != INDEX_NONE) {
			UE_LOG(LogTemp, Error, TEXT("Drawer: %s: Index at: %i Name: %s"), *Key.DrawerMesh->GetName(), ItemIndex,
			       *Key.DrawerItems[ItemIndex].Item->GetName())

			Key.DrawerItems.RemoveAt(ItemIndex);
			// break;
		}
	}
}
