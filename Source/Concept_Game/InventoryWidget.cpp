// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

void UInventoryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeOnMouseEnter1"))
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeOnMouseEnter2"))
}

//TODO: Check if bind dynamic call function when mouse is clicked
void UInventoryWidget::CreateOnMouseDownEvent() {
	UE_LOG(LogTemp, Warning, TEXT("CreateOnMouseDownEvent()"))
	MouseDownEvent.BindDynamic(this, &UInventoryWidget::OnMouseDownEvent);
}

FEventReply UInventoryWidget::OnMouseDownEvent(FGeometry Geometry, const FPointerEvent& Event) {
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnMouseDownEvent"))
	return {};
}
