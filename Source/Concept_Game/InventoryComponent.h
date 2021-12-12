// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInterface.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryLine {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D EndPoint;
};

USTRUCT(BlueprintType)
struct FInventoryTile {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UInventoryComponent : public UActorComponent, public IInventoryInterface {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void QuickSelectPieToggle(bool Visible);

	virtual void QuickSelectInteract();

	virtual void InventoryToggle();

	virtual void InventoryInteract();

	//TODO: Move to some base class to remove duplicates
	UFUNCTION(BlueprintCallable)
	FVector2D GetViewportCenter();

	UFUNCTION(BlueprintCallable)
	void SetQuickSelectPieWidgetSelection();

	//TODO: Move to abstract component that manages all ui actions
	UFUNCTION(BlueprintCallable)
	bool QuitActionButtonPressed();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta = (AllowPrivateAccess = "true"))
	bool bInventoryVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<class AItem*> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	FMatrix InventoryItemsPlacement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	TArray<class AItem*> QuickSelectItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility CurrentQuickSelectWidgetVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	class UPieMenu* QuickSelectPieWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSelect", meta = (AllowPrivateAccess = "true"))
	bool bQuickSelectVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSelect", meta = (AllowPrivateAccess = "true"))
	bool bQuickSelectVisibleRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory Menu", meta = (AllowPrivateAccess = "true"))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Menu", meta = (AllowPrivateAccess = "true"))
	int InventoryColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Menu", meta = (AllowPrivateAccess = "true"))
	int InventoryRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Menu", meta = (AllowPrivateAccess = "true"))
	float TileSize;

public:
	FORCEINLINE int GetInventoryColumns() const {
		return InventoryColumns;
	}

	FORCEINLINE int GetInventoryRows() const {
		return InventoryRows;
	}

	void SetInventoryColumns(int InColumns) {
		InventoryColumns = InColumns;
	}

	void SetInventoryRows(int InRows) {
		InventoryRows = InRows;
	}

	FORCEINLINE void AddInventoryItem(AItem* InventoryItem) {
		InventoryItems.Add(InventoryItem);
	}

	FORCEINLINE void RemoveInventoryItem(AItem* InventoryItem) {
		InventoryItems.Remove(InventoryItem);
	}

	FORCEINLINE void RemoveInventoryItemAtLocation(int32 Index) {
		InventoryItems.RemoveAt(Index);
	}

	FORCEINLINE AItem* GetInventoryItemAtLocation(int32 Index) {
		return InventoryItems[Index];
	}

	void ModifyInventoryItem(AItem* InventoryItem);

	void ModifyInventoryItemAtLocation(int32 Index);

	void CombineInventoryItems(AItem* InventoryItemA, AItem* InventoryItemB);

	void CombineInventoryItemsAtLocations(int32 IndexA, int32 IndexB);

	void SetToQuickSelect(AItem* InventoryItem);

	void RemoveFromQuickSelect(AItem* InventoryItem);

	UFUNCTION(BlueprintCallable)
	void CreateQuickSelectPieWidget(UPieMenu* InQuickSelectWidget);

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget(UInventoryWidget* InInventoryWidget);

	FORCEINLINE bool GetQuickSelectVisibility() const {
		return bQuickSelectVisible;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPieMenu* GetQuickSelectPieWidget() const {
		return QuickSelectPieWidget;
	}
};
