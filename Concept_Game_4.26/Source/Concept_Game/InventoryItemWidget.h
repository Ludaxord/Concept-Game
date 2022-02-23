// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedWidget, class AItem*, Item);


UCLASS()
class CONCEPT_GAME_API UInventoryItemWidget : public UUserWidget {
	GENERATED_BODY()

private:
	friend class UInventoryGridWidget;
	friend class UInventoryWidget;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Widgets", meta = (AllowPrivateAccess = "true"))
	FOnInputAction OnMouseEnterEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Widgets", meta = (AllowPrivateAccess = "true"))
	FOnInputAction OnMouseLeaveEvent;
public:
	UPROPERTY(BlueprintCallable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FOnItemRemovedWidget OnItemRemovedWidgetDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Widgets", meta = (AllowPrivateAccess = "true"))
	FOnInputAction MouseDownEvent;

	UFUNCTION(BlueprintCallable)
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnItemMouseButton(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

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

	void CreateEvents();

	UFUNCTION(BlueprintCallable)
	void OnDragItem();
};
