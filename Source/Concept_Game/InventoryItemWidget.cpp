// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "Item.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetMathLibrary.h"

void UInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	UE_LOG(LogTemp, Error, TEXT("NativeOnMouseEnter"))
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseEnter"))
}

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

void UInventoryItemWidget::SetIconImage(UMaterialInstance* Icon) {
	if (ItemImage && Icon) {

		FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(Icon,
		                                                                   UKismetMathLibrary::FTrunc(Size.X),
		                                                                   UKismetMathLibrary::FTrunc(Size.Y)
		);
		ItemImage->SetBrush(Brush);
		// ItemImage->SetBrushFromMaterial(Icon);
	}
}
