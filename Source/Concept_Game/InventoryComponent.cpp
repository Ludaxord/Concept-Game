// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "BaseHUDComponent.h"
#include "InventoryGridWidget.h"
#include "InventoryItemWidget.h"
#include "InventoryWidget.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "PieMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/PanelSlot.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() : bInventoryVisible(false),
                                             bQuickSelectVisible(false),
                                             bQuickSelectVisibleRef(false),
                                             bInventoryDirty(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::QuickSelectPieToggle(bool Visible) {
	if (OwningCharacter) {
		bQuickSelectVisible = Visible;
		QuickSelectInteract();
	}
}

void UInventoryComponent::QuickSelectInteract() {
	if (bQuickSelectVisible) {
		if (QuickSelectPieWidget) {
			if (bQuickSelectVisibleRef != bQuickSelectVisible) {
				Cast<AMainPlayerController>(OwningCharacter->GetController())->
					SetMouseLocation(GetViewportCenter().X, GetViewportCenter().Y);
				// Cast<AMainPlayerController>(OwningCharacter->GetController())->bShowMouseCursor = true;
				Cast<AMainPlayerController>(OwningCharacter->GetController())->ClientIgnoreMoveInput(true);
				Cast<AMainPlayerController>(OwningCharacter->GetController())->ClientIgnoreLookInput(true);
				// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(this, 0),
				//                                                QuickSelectPieWidget);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(
					Cast<AMainPlayerController>(OwningCharacter->GetController()),
					QuickSelectPieWidget);
				Cast<AMainPlayerController>(OwningCharacter->GetController())->
					SetInputModeGameOnly(false);

				QuickSelectPieWidget->SetSectorsToPieMenuWidget();
				bQuickSelectVisibleRef = bQuickSelectVisible;
			}

			QuickSelectPieWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			// QuickSelectWidget->AddToViewport();
			QuickSelectPieWidget->SetAlignmentInViewport(GetViewportCenter());
		}
	}
	else {
		if (QuickSelectPieWidget) {
			if (bQuickSelectVisibleRef != bQuickSelectVisible) {
				// Cast<AMainPlayerController>(OwningCharacter->GetController())->bShowMouseCursor = false;
				Cast<AMainPlayerController>(OwningCharacter->GetController())->ClientIgnoreMoveInput(false);
				Cast<AMainPlayerController>(OwningCharacter->GetController())->ClientIgnoreLookInput(false);
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(
					Cast<AMainPlayerController>(OwningCharacter->GetController()));
				bQuickSelectVisibleRef = bQuickSelectVisible;
			}

			QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
			// QuickSelectWidget->RemoveFromViewport();
		}
	}
}

void UInventoryComponent::InventoryToggle() {
	if (OwningCharacter) {
		bInventoryVisible = !bInventoryVisible;
		// InventoryInteract();
	}
}

void UInventoryComponent::InventoryInteract() {
	UE_LOG(LogTemp, Warning, TEXT("InventoryInteract"));
	if (InventoryWidget) {
		if (bInventoryVisible) {
			OwningCharacter->GetCharacterBaseHUDComponent()->SetBaseHUDVisibility(false);
			FInputModeGameAndUI InputMode = FInputModeGameAndUI();
			// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			// Cast<AMainPlayerController>(OwningCharacter->GetController())->SetInputMode(InputMode);
			/////
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(
				Cast<AMainPlayerController>(OwningCharacter->GetController()), InventoryWidget,
				EMouseLockMode::DoNotLock, false);
			/////

			InventoryWidget->SetInputActionPriority(9999);
			// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
			// Cast<AMainPlayerController>(OwningCharacter->GetController()), InventoryWidget,
			// EMouseLockMode::DoNotLock);
			/////

			Cast<AMainPlayerController>(OwningCharacter->GetController())->bShowMouseCursor = true;
			// UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(true);
			// UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(true);
			InventoryWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			InventoryWidget->SetAlignmentInViewport(GetViewportCenter());
			InventoryWidget->ListenForInputAction(TEXT("UseWeapon"), IE_Pressed, true, InventoryWidget->MouseDownEvent);
		}
		else {
			OwningCharacter->GetCharacterBaseHUDComponent()->SetBaseHUDVisibility(true);
			InventoryWidget->SetInputActionPriority(-999);
			// Cast<AMainPlayerController>(OwningCharacter->GetController())->SetInputMode(FInputModeGameOnly());
			Cast<AMainPlayerController>(OwningCharacter->GetController())->bShowMouseCursor = false;
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(
				Cast<AMainPlayerController>(OwningCharacter->GetController()));
			// Cast<AMainPlayerController>(OwningCharacter->GetController())->bShowMouseCursor = false;
			Cast<AMainPlayerController>(OwningCharacter->GetController())->
				SetInputModeGameOnly(false);

			// UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(false);
			// UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(false);
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			InventoryWidget->StopListeningForInputAction(TEXT("UseWeapon"), IE_Pressed);
		}
	}
	else {
		Cast<AMainPlayerController>(OwningCharacter->GetController())->SetInputMode(FInputModeGameOnly());
	}
}

void UInventoryComponent::RemoveGridItem(AItem* InItem) {
}


// Called when the game starts
void UInventoryComponent::BeginPlay() {
	Super::BeginPlay();

	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	// ...
	InventoryItems.Init(nullptr, InventoryColumns * InventoryRows);
	// InventoryItems.SetNum(InventoryColumns * InventoryRows);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshInventoryWidget();
}

FVector2D UInventoryComponent::GetViewportCenter() {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
}

void UInventoryComponent::SetQuickSelectPieWidgetSelection() {
	if (QuickSelectPieWidget) {
		QuickSelectPieWidget->UpdatePieMenuSector();
		// QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UInventoryComponent::QuitActionButtonPressed() {
	bool bInteract = false;
	if (bInventoryVisible) {
		InventoryToggle();
		bInteract = true;
	}

	return bInteract;
}

void UInventoryComponent::CreateQuickSelectPieWidget(UPieMenu* InQuickSelectWidget) {
	QuickSelectPieWidget = InQuickSelectWidget;
	QuickSelectPieWidget->SetOwnerInventoryComponent(this);
	QuickSelectPieWidget->AddToViewport();
	QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryComponent::CreateInventoryWidget(UInventoryWidget* InInventoryWidget,
                                                UInventoryGridWidget* InInventoryGridWidget,
                                                UBorder* InBackgroundBorder) {
	InventoryWidget = InInventoryWidget;
	InventoryWidget->SetOwnerInventoryComponent(this);
	InventoryWidget->CreateOnMouseDownEvent();
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->SetInventoryGridWidget(InInventoryGridWidget);
	InventoryWidget->SetBackgroundBorder(InBackgroundBorder);
}

void UInventoryComponent::MouseButtonPressed() {
	InventoryWidget->SetFocus();
	for (UInventoryItemWidget* Widget : InventoryWidget->InventoryGridWidget->InventoryItemWidgets) {
		Widget->SetFocus();
		Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

TMap<AItem*, FInventoryTile> UInventoryComponent::GetInventoryItems() {
	TMap<AItem*, FInventoryTile> AllItems;
	for (int i = 0; i <= InventoryItems.Num(); i++) {
		if (InventoryItems.IsValidIndex(i)) {
			AItem* InvItem = InventoryItems[i];
			if (InvItem != nullptr)
				// UE_LOG(LogTemp, Error, TEXT("IsNotItemNull__: %s"), InvItem != nullptr ? TEXT("true") : TEXT("false"))
				if (InvItem != nullptr) {
					// UE_LOG(LogTemp, Warning, TEXT("InventoryItem Current Item: %s Valid Index: %i"), *InvItem->GetName(), i)
					if (!AllItems.Contains(InvItem)) {
						AllItems.Add(InvItem, IndexToTile(i));
					}
				}
		}
	}

	return AllItems;
}

FInventoryTile UInventoryComponent::IndexToTile(int Index) const {
	return {Index % InventoryColumns, Index / InventoryColumns};
}

int UInventoryComponent::TileToIndex(FInventoryTile InTile) const {
	return InTile.X + (InTile.Y * InventoryColumns);
}

FInventoryTile UInventoryComponent::OutputEachTile(AItem* InInventoryItem, int TopLeftIndex) {
	return {};
}

bool UInventoryComponent::CheckInventorySpace(AItem* InInventoryItem, int TopLeftIndex) {
	const FIntPoint ItemDimension = InInventoryItem->GetItemDimensions();
	const FInventoryTile Tiles = IndexToTile(TopLeftIndex);
	// UE_LOG(LogTemp, Warning, TEXT("Check Inventory Space: %s Tiles: X -> %i Y -> %i"), *ItemDimension.ToString(),
	       // Tiles.X, Tiles.Y)
	for (int i = Tiles.X; i <= Tiles.X + (ItemDimension.X - 1); i++) {
		for (int j = Tiles.Y; j <= Tiles.Y + (ItemDimension.Y - 1); j++) {
			const FInventoryTile NewTile = {i, j};
			if (NewTile.X >= 0 && NewTile.Y >= 0 && NewTile.X < InventoryColumns && NewTile.Y < InventoryRows) {
				if (GetItemAtIndex(TileToIndex(NewTile)) != nullptr || IsValid(GetItemAtIndex(TileToIndex(NewTile)))) {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}

	return true;

}

bool UInventoryComponent::TryAddInventoryItem(AItem* InInventoryItem) {
	if (IsValid(InInventoryItem)) {
		// int i = 0;
		// for (AItem* InventoryItem : InventoryItems) {
		for (int i = 0; i <= InventoryItems.Num(); i++) {
			if (CheckInventorySpace(InInventoryItem, i)) {
				UE_LOG(LogTemp, Error, TEXT("TryAddInventoryItem -> %i"), i)
				return AddInventoryItem(InInventoryItem, i);
			}
			// i++;
		}
	}

	return false;
}

bool UInventoryComponent::AddInventoryItem(AItem* InInventoryItem, int TopLeftIndex) {
	const FIntPoint ItemDimension = InInventoryItem->GetItemDimensions();
	const FInventoryTile Tiles = IndexToTile(TopLeftIndex);
	for (int i = Tiles.X; i <= Tiles.X + (ItemDimension.X - 1); i++) {
		for (int j = Tiles.Y; j <= Tiles.Y + (ItemDimension.Y - 1); j++) {
			const FInventoryTile NewTile = {i, j};
			InventoryItems[TileToIndex(NewTile)] = InInventoryItem;
			InInventoryItem->SetItemState(EItemState::EIS_Equipped);
			// InventoryItems.EmplaceAt(TileToIndex(NewTile), InInventoryItem);
			bInventoryDirty = true;
			// AItem* AddedItem = InventoryItems[TileToIndex(NewTile)];
			// UE_LOG(LogTemp, Warning, TEXT("Current Inventory Item added at %i for Tile {%i, %i}"), TileToIndex(NewTile),
			//        i, j)
		}
	}

	return bInventoryDirty;
}

AItem* UInventoryComponent::GetItemAtIndex(int InIndex) {
	if (InventoryItems.IsValidIndex(InIndex)) {
		return InventoryItems[InIndex];
	}

	return nullptr;
}

bool UInventoryComponent::RemoveInventoryItem(AItem* InInventoryItem) {
	bool bItemRemoved = false;
	if (InInventoryItem != nullptr) {
		for (int i = 0; i <= InventoryItems.Num(); i++) {
			if (InventoryItems.IsValidIndex(i)) {
				if (InInventoryItem == InventoryItems[i]) {
					// InventoryItems.RemoveAt(i, 1, false);
					InventoryItems[i] = nullptr;
					bInventoryDirty = true;
					bItemRemoved = true;
					// UE_LOG(LogTemp, Warning, TEXT("Found Item To Remove: %s Still Exists: %s at: %i"),
					//        *InInventoryItem->GetName(),
					//        InventoryItems.IsValidIndex(i) ? TEXT("true") : TEXT("false"),
					//        i
					// )
				}
			}
		}

		//if player is holding dropped item, drop it (it will change pose of character)
		// if (InInventoryItem == OwningCharacter->GetEquippedWeapon()) {
		// 	OwningCharacter->DropItem(InInventoryItem);
		// 	OwningCharacter->SetEquippedWeapon(nullptr);
		// }
	}

	return bItemRemoved;
}

bool UInventoryComponent::RemoveCurrentEquippedItem(AItem* InInventoryItem) {
	//if player is holding dropped item, drop it (it will change pose of character)
	if (InInventoryItem == OwningCharacter->GetEquippedWeapon()) {
		OwningCharacter->DropItem(InInventoryItem);
		OwningCharacter->SetEquippedWeapon(nullptr);
		return true;
	}

	return false;
}

bool UInventoryComponent::RemoveInventoryItemAtLocation(int32 Index) {
	return false;
}

AItem* UInventoryComponent::GetInventoryItemAtLocation(int32 Index) {
	return nullptr;
}

void UInventoryComponent::ModifyInventoryItem(AItem* InventoryItem) {
}

void UInventoryComponent::ModifyInventoryItemAtLocation(int32 Index) {
}

void UInventoryComponent::CombineInventoryItems(AItem* InventoryItemA, AItem* InventoryItemB) {
}

void UInventoryComponent::CombineInventoryItemsAtLocations(int32 IndexA, int32 IndexB) {
}

void UInventoryComponent::SetToQuickSelect(AItem* InventoryItem) {
}

void UInventoryComponent::RemoveFromQuickSelect(AItem* InventoryItem) {
}

void UInventoryComponent::CallInventoryInteraction(FPointerEvent Event, UInventoryItemWidget* Widget, FKey Key) {
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(Event, Widget, Key);
	auto ReplyString = Reply.NativeReply.ToString();
	UE_LOG(LogTemp, Warning, TEXT("ReplyString %s"), *ReplyString)
}

void UInventoryComponent::RefreshInventoryWidget() {
	if (bInventoryDirty) {
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting RefreshSpatialGridWidgetDelegate"))
		RefreshSpatialGridWidgetDelegate.Broadcast();
		// RefreshGridWidgetDelegate.Broadcast(ItemWidgetSubclass);
		bInventoryDirty = false;
	}
}
