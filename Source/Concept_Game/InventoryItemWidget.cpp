// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "Item.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UInventoryItemWidget::Refresh(UImage* InItemImage, USizeBox* InBackgroundSizeBox,
                                   UBorder* InBackgroundBorder) {
	ItemImage = InItemImage;
	BackgroundSizeBox = InBackgroundSizeBox;
	BackgroundBorder = InBackgroundBorder;

	FIntPoint Dimensions = Item->GetItemDimensions();
	float DimensionsX = Dimensions.X * TileSize;
	float DimensionsY = Dimensions.Y * TileSize;
	Size = {DimensionsX, DimensionsY};
	BackgroundSizeBox->SetWidthOverride(Size.X);
	BackgroundSizeBox->SetHeightOverride(Size.Y);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage)->SetSize(Size);

}
