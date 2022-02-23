// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryGridWidget.h"
#include "InventoryItemWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"

//TODO: Check if bind dynamic call function when mouse is clicked
void UInventoryWidget::CreateOnMouseDownEvent() {
	UE_LOG(LogTemp, Warning, TEXT("CreateOnMouseDownEvent()"))
	MouseDownEvent.BindDynamic(this, &UInventoryWidget::OnMouseDownEvent);
}

void UInventoryWidget::OnMouseDownEvent() {
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnMouseDownEvent"))
	InventoryGridWidget->SetFocus();
	// if (UCanvasPanel* OverlayWidget = Cast<UCanvasPanel>(GetRootWidget())) {
	// 	int32 ChildrenCount = OverlayWidget->GetChildrenCount();
	// 	for (int i = 0; i < ChildrenCount; i++) {
	// 		if (UInventoryGridWidget* InvGridWidget = Cast<UInventoryGridWidget>(OverlayWidget->GetChildAt(i))) {
	// 			if (UCanvasPanel* CanvasWidget = Cast<UCanvasPanel>(InvGridWidget->GetRootWidget())) {
	// 				for (int j = 0; j < CanvasWidget->GetChildrenCount(); j++) {
	// 					if (UBorder* BorderWidget = Cast<UBorder>(CanvasWidget->GetChildAt(j))) {
	// 						for (int k = 0; k < BorderWidget->GetChildrenCount(); k++) {
	// 							if (UCanvasPanel* CanvasPanelWidget = Cast<UCanvasPanel>(BorderWidget->GetChildAt(k))) {
	// 								for (int n = 0; n < CanvasPanelWidget->GetChildrenCount(); n++) {
	// 									if (UInventoryItemWidget* InvItemWidget = Cast<UInventoryItemWidget>(
	// 										CanvasPanelWidget->GetChildAt(n))) {
	// 										InvItemWidget->bIsFocusable = true;
	// 										InvItemWidget->SetFocus();
	// 										InvItemWidget->SetVisibility(ESlateVisibility::Visible);
	// 										InvItemWidget->ListenForInputAction(
	// 											TEXT("UseWeapon"), IE_Pressed, true, InvItemWidget->MouseDownEvent);
	//
	// 										// FPointerEvent Event = {};
	// 										// Event.IsMouseButtonDown(FKey("LeftMouseButton"));
	// 										// InvItemWidget->
	// 										// 	NativeOnMouseEnter(InvItemWidget->GetCachedGeometry(), Event);
	// 										// InvItemWidget->OnMouseEnter(InvItemWidget->GetCachedGeometry(), Event);
	// 										// InvItemWidget->OnMouseButtonDown(InvItemWidget->GetCachedGeometry(), Event);
	// 										// CallInventoryInteraction(Event, InvItemWidget, FKey("LeftMouseButton"));
	// 									}
	// 								}
	// 							}
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }
}

FEventReply UInventoryWidget::OnMouseDownEventCallable(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	UE_LOG(LogTemp, Warning,
	       TEXT("UInventoryWidget::OnMouseDownEvent(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)"))
	return FEventReply(true);
}
