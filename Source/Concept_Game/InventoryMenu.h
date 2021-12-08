// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryMenu : public UUserWidget {
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory SubWidgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* ArrowWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* OwnerInventoryComponent;

public:
	UFUNCTION(BlueprintCallable)
	void SetArrowWidget(UUserWidget* InArrowWidget) {
		ArrowWidget = InArrowWidget;
	}

	UFUNCTION(BlueprintCallable)
	void SetOwnerInventoryComponent(UInventoryComponent* InOwnerInventoryComponent) {
		OwnerInventoryComponent = InOwnerInventoryComponent;
	}

	UUserWidget* GetArrowWidget() {
		return ArrowWidget;
	}
};
