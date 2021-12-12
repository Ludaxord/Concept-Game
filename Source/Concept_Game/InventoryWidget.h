// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* OwnerInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Properties", meta = (AllowPrivateAccess = "true"))
	float TileSize;

public:
	UFUNCTION(BlueprintCallable)
	void SetOwnerInventoryComponent(UInventoryComponent* InOwnerInventoryComponent) {
		OwnerInventoryComponent = InOwnerInventoryComponent;
	}
};
