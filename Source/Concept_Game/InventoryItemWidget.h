// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryItemWidget : public UUserWidget {
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class UBorder* BackgroundBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class USizeBox* BackgroundSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class AItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D Size;

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//
	// virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	//
	// virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	FORCEINLINE void SetTileSize(float InTileSize) {
		TileSize = InTileSize;
	}

	FORCEINLINE void SetItem(AItem* InItem) {
		Item = InItem;
	}

	FORCEINLINE AItem* GetItem() const {
		return Item;
	}

	UFUNCTION(BlueprintCallable)
	void Refresh(class UImage* InItemImage,
	             class USizeBox* InBackgroundSizeBox,
	             class UBorder* InBackgroundBorder);

	UFUNCTION(BlueprintCallable)
	void SetIconImage(class UMaterialInstance* Icon);
};
