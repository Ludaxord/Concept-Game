// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUDComponent.h"

#include "BasePlayerHUDWidget.h"
#include "MainCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBaseHUDComponent::UBaseHUDComponent(): bBaseHUDVisible(true) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseHUDComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());

	// ...

}


// Called every frame
void UBaseHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HUDVisibilityState();
}

FVector2D UBaseHUDComponent::GetViewportCenter() {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
}

void UBaseHUDComponent::CreateBasePlayerHUD(UBasePlayerHUDWidget* InPlayerHUDWidget) {
	PlayerHUDWidget = InPlayerHUDWidget;
	PlayerHUDWidget->AddToViewport();
	PlayerHUDWidget->SetVisibility(bBaseHUDVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UBaseHUDComponent::HUDVisibilityState() {
	if (OwningCharacter) {
		bBaseHUDVisible = OwningCharacter->GetPlayerHUDVisibility();
	}

	if (PlayerHUDWidget) {
		PlayerHUDWidget->SetVisibility(bBaseHUDVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}
