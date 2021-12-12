// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridWidget.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryGridWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Create(UInventoryComponent* InInventoryComponent, float InTileSize, class UBorder* InInventoryGridBorder,
	            class UCanvasPanel* InInventoryCanvasPanel);

private:
	void SetGridBorderSize() const;

	void CreateLineSegments();

	UFUNCTION(BlueprintCallable)
	void DrawLines(FPaintContext Context);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* OwnerInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class UBorder* InventoryGridBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	class UCanvasPanel* InventoryCanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	TArray<FInventoryLine> Lines;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	UInventoryGridWidget* InventoryGridWidgetRef;

};
