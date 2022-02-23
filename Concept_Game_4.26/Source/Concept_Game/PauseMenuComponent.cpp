// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuComponent.h"

#include "MainCharacter.h"
#include "PauseMenuWidget.h"

// Sets default values for this component's properties
UPauseMenuComponent::UPauseMenuComponent(): bPauseMenuVisible(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPauseMenuComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	// ...

}


// Called every frame
void UPauseMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PauseVisibilityState();
}

FVector2D UPauseMenuComponent::GetViewportCenter() {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
}

void UPauseMenuComponent::CreatePauseMenu(UPauseMenuWidget* InPauseMenuWidget) {
	PauseMenuWidget = InPauseMenuWidget;
	PauseMenuWidget->AddToViewport();
	PauseMenuWidget->SetVisibility(bPauseMenuVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UPauseMenuComponent::PauseVisibilityState() {

}

void UPauseMenuComponent::PauseMenuToggle() {
	if (OwningCharacter) {
		bPauseMenuVisible = !bPauseMenuVisible;
		UE_LOG(LogTemp, Warning, TEXT("Pause Toggle"));
	}

	if (PauseMenuWidget) {
		PauseMenuWidget->SetAlignmentInViewport(GetViewportCenter());
		PauseMenuWidget->SetVisibility(
			bPauseMenuVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}
