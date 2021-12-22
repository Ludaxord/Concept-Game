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
	FOnInputAction MouseDownEvent;

public:
	UFUNCTION(BlueprintCallable)
	void CreateOnMouseDownEvent();

	UFUNCTION(BlueprintCallable)
	void OnMouseDownEvent();

	UFUNCTION(BlueprintCallable)
	FEventReply OnMouseDownEventCallable(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

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
