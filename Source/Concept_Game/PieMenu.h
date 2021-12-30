// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PieMenu.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UPieMenu : public UUserWidget {
	GENERATED_BODY()

public:
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* OwnerInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* PieMenuMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	int SectorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	int SectorSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	float CurrentSectorAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	TArray<int> SectorListMaxBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component", meta = (AllowPrivateAccess = "true"))
	TArray<int> SectorListMinBounds;
public:
	UFUNCTION(BlueprintCallable)
	void SetSectorsToPieMenuWidget();

	UFUNCTION(BlueprintCallable)
	void GetSectorBounds(int InSectorCount);

	UFUNCTION(BlueprintCallable)
	void UpdatePieMenuSector();

	UFUNCTION(BlueprintCallable)
	float GetCurrentSector(float InCurrentAngle);

	UFUNCTION(BlueprintCallable)
	float GetProperRotation();

	UFUNCTION(BlueprintCallable)
	void SetOwnerInventoryComponent(UInventoryComponent* InOwnerInventoryComponent) {
		OwnerInventoryComponent = InOwnerInventoryComponent;
	}

	UFUNCTION(BlueprintCallable)
	void SetPieMenuMaterialInstance(UMaterialInstanceDynamic* InPieMenuMaterialInstance) {
		PieMenuMaterialInstance = InPieMenuMaterialInstance;
	}

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetPieMenuMaterialInstance() {
		return PieMenuMaterialInstance;
	}

	FORCEINLINE int GetSectorCount() const {
		return SectorCount;
	}
};
