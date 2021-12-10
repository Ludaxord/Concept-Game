// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryMenu.h"
#include "MainCharacter.h"
#include "PieMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() : bInventoryVisible(false), bQuickSelectVisible(false),
                                             bQuickSelectVisibleRef(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//TODO: Remove
void UInventoryComponent::QuickSelectToggle(bool Visible) {
	if (OwningCharacter) {
		bQuickSelectVisible = Visible;
		if (bQuickSelectVisible) {
			if (QuickSelectWidget) {
				if (bQuickSelectVisibleRef != bQuickSelectVisible) {
					UE_LOG(LogTemp, Warning, TEXT("Ignore Move Input"))
					UGameplayStatics::GetPlayerController(this, 0)->SetMouseLocation(
						GetViewportCenter().X, GetViewportCenter().Y);
					UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(true);
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(true);
					bQuickSelectVisibleRef = bQuickSelectVisible;
				}

				QuickSelectWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
				// QuickSelectWidget->AddToViewport();
				QuickSelectWidget->SetAlignmentInViewport(GetViewportCenter());
			}
		}
		else {
			if (QuickSelectWidget) {
				if (bQuickSelectVisibleRef != bQuickSelectVisible) {
					UE_LOG(LogTemp, Warning, TEXT("NOT Ignore Move Input"))
					UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(false);
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(false);
					bQuickSelectVisibleRef = bQuickSelectVisible;
				}

				QuickSelectWidget->SetVisibility(ESlateVisibility::Hidden);
				// QuickSelectWidget->RemoveFromViewport();
			}
		}
	}
}

void UInventoryComponent::QuickSelectPieToggle(bool Visible) {
	if (OwningCharacter) {
		bQuickSelectVisible = Visible;
		if (bQuickSelectVisible) {
			if (QuickSelectPieWidget) {
				if (bQuickSelectVisibleRef != bQuickSelectVisible) {
					UE_LOG(LogTemp, Warning, TEXT("Ignore Move Input"))
					UGameplayStatics::GetPlayerController(this, 0)->SetMouseLocation(
						GetViewportCenter().X, GetViewportCenter().Y);
					// UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(true);
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(true);
					bQuickSelectVisibleRef = bQuickSelectVisible;
				}

				QuickSelectPieWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
				// QuickSelectWidget->AddToViewport();
				QuickSelectPieWidget->SetAlignmentInViewport(GetViewportCenter());
			}
		}
		else {
			if (QuickSelectPieWidget) {
				if (bQuickSelectVisibleRef != bQuickSelectVisible) {
					UE_LOG(LogTemp, Warning, TEXT("NOT Ignore Move Input"))
					// UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreMoveInput(false);
					UGameplayStatics::GetPlayerController(this, 0)->ClientIgnoreLookInput(false);
					bQuickSelectVisibleRef = bQuickSelectVisible;
				}

				QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
				// QuickSelectWidget->RemoveFromViewport();
			}
		}
	}
}

void UInventoryComponent::InventoryToggle() {
	UE_LOG(LogTemp, Warning, TEXT("Inventory Toggle"));
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
}

//TODO: Remove
FVector2D UInventoryComponent::GetViewportCenter() {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return {ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f};
}

//TODO: Remove
void UInventoryComponent::QuickSelectInteractions() {
	if (bQuickSelectVisible) {
		float MouseRot = GetMouseRotationInViewport();
		if (QuickSelectWidget != nullptr) {
			SetQuickSelectArrowAngle(QuickSelectWidget, MouseRot);
			// if (QuickSelectWidget->GetArrowWidget() != nullptr) {
			// SetQuickSelectArrowAngle(QuickSelectWidget->GetArrowWidget(), MouseRot);
			// }
		}
	}
}

void UInventoryComponent::SetQuickSelectPieWidgetSelection() {
	if (QuickSelectPieWidget) {
		QuickSelectPieWidget->UpdatePieMenuSector();
		// QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

//TODO: Remove
float UInventoryComponent::GetMouseRotationInViewport() {
	FVector2D ViewportCenter = GetViewportCenter();
	// FVector2D ViewportCenter = {QuickSelectViewportSize.X / 2.f, QuickSelectViewportSize.Y / 2.f};
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(OwningCharacter->GetWorld());
	FVector2D ViewportCenterX = {ViewportCenter.X, 0.0f};

	FVector2D MousePositionDiff = ViewportCenter - MousePosition;
	UKismetMathLibrary::Normalize2D(MousePositionDiff);

	FVector2D ViewportCenterXDiff = ViewportCenterX - ViewportCenter;
	UKismetMathLibrary::Normalize2D(ViewportCenterXDiff);

	float ViewportMousePositionDotProd = UKismetMathLibrary::DotProduct2D(MousePositionDiff, ViewportCenterXDiff);
	float ViewportMousePositionDotProdACos = UKismetMathLibrary::DegAcos(ViewportMousePositionDotProd);

	bool bSelect = MousePosition.X - ViewportCenter.X >= 0.0f;
	float SelectedFloat = UKismetMathLibrary::SelectFloat(1.0f, -1.0f, bSelect);

	UE_LOG(LogTemp, Warning,
	       TEXT(
		       "Get MouseRotation In Viewport: %f ViewportMousePositionDotProd: %f MousePosition: %s MousePosition.X - ViewportCenter.X: %f ViewportCenter: %s"
	       ),
	       SelectedFloat * ViewportMousePositionDotProdACos,
	       ViewportMousePositionDotProd,
	       *MousePosition.ToString(),
	       MousePosition.X - ViewportCenter.X,
	       *ViewportCenter.ToString()
	)

	return SelectedFloat * ViewportMousePositionDotProdACos;
}

//TODO: Remove
void UInventoryComponent::SetQuickSelectArrowAngle(UUserWidget* ArrowWidget, float InAngle) {
	if (QuickSelectWidget && ArrowWidget) {
		ArrowWidget->SetRenderTransformAngle(InAngle);
	}
}

//TODO: Remove
FIntPoint UInventoryComponent::SetViewportSizeForQuickSelect() {
	TArray<FIntPoint> ViewportSizes;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(ViewportSizes);
	if (ViewportSizes.Num() <= 0) {
		return FIntPoint();
	}

	return ViewportSizes[ViewportSizes.Num() - 1];
}

//TODO: Remove
void UInventoryComponent::CreateQuickSelectWidget(UInventoryMenu* InQuickSelectWidget) {
	QuickSelectWidget = InQuickSelectWidget;
	QuickSelectWidget->SetOwnerInventoryComponent(this);
	QuickSelectWidget->AddToViewport();
	QuickSelectWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryComponent::CreateQuickSelectPieWidget(UPieMenu* InQuickSelectWidget) {
	QuickSelectPieWidget = InQuickSelectWidget;
	QuickSelectPieWidget->SetOwnerInventoryComponent(this);
	QuickSelectPieWidget->AddToViewport();
	QuickSelectPieWidget->SetVisibility(ESlateVisibility::Hidden);
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
