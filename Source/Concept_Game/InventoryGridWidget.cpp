// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"

#include "InventoryComponent.h"
#include "InventoryItemWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UInventoryGridWidget::Create(UInventoryComponent* InInventoryComponent, float InTileSize,
                                  UBorder* InInventoryGridBorder,
                                  UCanvasPanel* InInventoryCanvasPanel) {
	InventoryGridWidgetRef = this;
	OwnerInventoryComponent = InInventoryComponent;
	TileSize = InTileSize;
	InventoryGridBorder = InInventoryGridBorder;
	InventoryCanvasPanel = InInventoryCanvasPanel;
	SetGridBorderSize();
	CreateLineSegments();
}

void UInventoryGridWidget::SetGridBorderSize() const {
	if (InventoryGridBorder) {
		if (OwnerInventoryComponent) {
			UWidgetLayoutLibrary::SlotAsCanvasSlot(InventoryGridBorder)->SetSize({
				OwnerInventoryComponent->GetInventoryColumns() * TileSize,
				OwnerInventoryComponent->GetInventoryRows() * TileSize
			});
		}
	}
}

void UInventoryGridWidget::RefreshWidget(TSubclassOf<UInventoryItemWidget> WidgetSubclass) {
	if (OwnerInventoryComponent) {
		UE_LOG(LogTemp, Error, TEXT("Refreshing Grid..."))
		InventoryCanvasPanel->ClearChildren();
		TMap<AItem*, FInventoryTile> InventoryItemsMap = OwnerInventoryComponent->GetInventoryItems();
		TArray<AItem*> ItemKeys;
		InventoryItemsMap.GetKeys(ItemKeys);
		for (AItem* Key : ItemKeys) {
			FInventoryTile* TileValue = InventoryItemsMap.Find(Key);
			UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(GetWorld(), WidgetSubclass);
			ItemWidget->SetItem(Key);
			ItemWidget->SetTileSize(TileSize);
			//TODO: Add OnRemoved Bind Event Function
			UPanelSlot* PanelSlot = InventoryCanvasPanel->AddChild(ItemWidget);
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot);
			CanvasSlot->SetAutoSize(true);
			CanvasSlot->SetPosition({TileValue->X * TileSize, TileValue->Y * TileSize});
		}
	}
}

void UInventoryGridWidget::BindInventoryComponentEvents(TSubclassOf<UInventoryItemWidget> WidgetSubclass) {
	if (OwnerInventoryComponent) {
		OwnerInventoryComponent->SetItemWidgetSubclass(WidgetSubclass);
		OwnerInventoryComponent->RefreshGridWidgetDelegate.AddDynamic(this, &UInventoryGridWidget::RefreshWidget);
	}
}

void UInventoryGridWidget::CreateLineSegments() {
	if (InventoryGridBorder) {
		if (OwnerInventoryComponent) {
			for (int i = 0; i <= OwnerInventoryComponent->GetInventoryColumns(); i++) {
				float X_ = i * TileSize;
				FInventoryLine Line = FInventoryLine();
				Line.StartPoint = {X_, 0.0f};
				Line.EndPoint = {X_, OwnerInventoryComponent->GetInventoryRows() * TileSize};
				Lines.Add(Line);
			}

			for (int i = 0; i <= OwnerInventoryComponent->GetInventoryRows(); i++) {
				float Y_ = i * TileSize;
				FInventoryLine Line = FInventoryLine();
				Line.StartPoint = {0.0f, Y_};
				Line.EndPoint = {OwnerInventoryComponent->GetInventoryColumns() * TileSize, Y_};
				Lines.Add(Line);
			}
		}
	}
}

void UInventoryGridWidget::DrawLines(FPaintContext Context) {
	if (InventoryGridBorder) {
		int i = 0;
		for (FInventoryLine Line : Lines) {
			UWidgetBlueprintLibrary::DrawLine(Context,
			                                  USlateBlueprintLibrary::GetLocalTopLeft(
				                                  InventoryGridBorder->GetCachedGeometry()) + Line.StartPoint,
			                                  USlateBlueprintLibrary::GetLocalTopLeft(
				                                  InventoryGridBorder->GetCachedGeometry()) + Line.EndPoint,
			                                  FLinearColor(0.5f, 0.5f, 0.5f, 0.5f)
			);

			i++;
		}
	}
}
