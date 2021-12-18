// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

void UInventoryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeOnMouseEnter1"))
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeOnMouseEnter2"))
}
