// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryWidget : public UUserWidget {
	GENERATED_BODY()
private:
	friend class UInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* OwnerInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryGridWidget* InventoryGridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UBorder* BackgroundBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Widgets", meta = (AllowPrivateAccess = "true"))
	FOnPointerEvent MouseDownEvent;

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	void CreateOnMouseDownEvent();

	UFUNCTION()
	FEventReply OnMouseDownEvent(FGeometry Geometry, const FPointerEvent& Event);

	UFUNCTION(BlueprintCallable)
	void SetOwnerInventoryComponent(UInventoryComponent* InOwnerInventoryComponent) {
		OwnerInventoryComponent = InOwnerInventoryComponent;
	}

	UFUNCTION(BlueprintCallable)
	void SetInventoryGridWidget(class UInventoryGridWidget* InInventoryGridWidget) {
		InventoryGridWidget = InInventoryGridWidget;
	}

	UFUNCTION(BlueprintCallable)
	void SetBackgroundBorder(UBorder* InBackgroundBorder) {
		BackgroundBorder = InBackgroundBorder;
	}
};
