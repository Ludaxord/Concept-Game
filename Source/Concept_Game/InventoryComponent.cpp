// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryMenu.h"
#include "MainCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() : bInventoryVisible(false), bQuickSelectVisible(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::QuickSelectToggle(bool Visible) {
	if (OwningCharacter) {
		bQuickSelectVisible = Visible;
		if (bQuickSelectVisible) {
			if (QuickSelectWidget) {
				UE_LOG(LogTemp, Warning, TEXT("Quick Select Toggle"));
				QuickSelectWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
				// QuickSelectWidget->AddToViewport();
				QuickSelectWidget->SetAlignmentInViewport(GetViewportCenter());
			}
		}
		else {
			if (QuickSelectWidget) {
				QuickSelectWidget->SetVisibility(ESlateVisibility::Hidden);
				// QuickSelectWidget->RemoveFromViewport();
			}
		}
	}
}

void UInventoryComponent::InventoryToggle() {
	if (OwningCharacter) {
		bInventoryVisible = !bInventoryVisible;
		UE_LOG(LogTemp, Warning, TEXT("Inventory Toggle"));
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay() {
	Super::BeginPlay();

	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	// ...

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector2D UInventoryComponent::GetViewportCenter() {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
}

void UInventoryComponent::CreateQuickSelectWidget(UInventoryMenu* InQuickSelectWidget) {
	QuickSelectWidget = InQuickSelectWidget;
	QuickSelectWidget->AddToViewport();
	QuickSelectWidget->Visibility = ESlateVisibility::Hidden;
}

void UInventoryComponent::ModifyInventoryItem(AItem* InventoryItem) {
}

void UInventoryComponent::ModifyInventoryItemAtLocation(int32 Index) {
}

void UInventoryComponent::CombineInventoryItems(AItem* InventoryItemA, AItem* InventoryItemB) {
}

void UInventoryComponent::CombineInventoryItemsAtLocations(int32 IndexA, int32 IndexB) {
}

void UInventoryComponent::SetToQuickSelect(AItem* InventoryItem) {
}

void UInventoryComponent::RemoveFromQuickSelect(AItem* InventoryItem) {
}
