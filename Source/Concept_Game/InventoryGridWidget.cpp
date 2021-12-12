// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"

#include "InventoryComponent.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
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

void UInventoryGridWidget::CreateLineSegments() {
	if (InventoryGridBorder) {
		if (OwnerInventoryComponent) {
			for (int i = 0; i < OwnerInventoryComponent->GetInventoryColumns(); i++) {
				float X_ = i * TileSize;
				FInventoryLine Line = FInventoryLine();
				Line.StartPoint = {X_, 0.0f};
				Line.EndPoint = {X_, OwnerInventoryComponent->GetInventoryColumns() * TileSize};
				Lines.Add(Line);
			}

			for (int i = 0; i < OwnerInventoryComponent->GetInventoryRows(); i++) {
				float Y_ = i * TileSize;
				FInventoryLine Line = FInventoryLine();
				Line.StartPoint = {0.0f, Y_};
				Line.EndPoint = {OwnerInventoryComponent->GetInventoryRows() * TileSize, Y_};
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
