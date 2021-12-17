// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController() {
}

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (HUDOverlayClass) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if (HUDOverlay) {
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::SetInputModeGameOnly(bool InConsumeCaptureMouseDown) {
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(
		InConsumeCaptureMouseDown);
	SetInputMode(InputMode);
}
