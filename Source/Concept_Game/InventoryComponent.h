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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRefreshGridWidget, TSubclassOf<class UInventoryItemWidget>, WidgetSubclass)
;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta = (AllowPrivateAccess = "true"))
	bool bInventoryDirty;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItemWidget> ItemWidgetSubclass;

public:
	UPROPERTY(BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FRefreshGridWidget RefreshGridWidgetDelegate;

	FORCEINLINE void SetItemWidgetSubclass(TSubclassOf<UInventoryItemWidget> InItemWidgetSubclass) {
		ItemWidgetSubclass = InItemWidgetSubclass;
	}

	FORCEINLINE FRefreshGridWidget GetRefreshGridWidgetDelegate() {
		return RefreshGridWidgetDelegate;
	}

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

	void MouseButtonPressed();

	TMap<AItem*, FInventoryTile> GetInventoryItems();

	FInventoryTile IndexToTile(int Index) const;

	int TileToIndex(FInventoryTile InTile) const;

	FInventoryTile OutputEachTile(AItem* InInventoryItem, int TopLeftIndex);

	bool CheckInventorySpace(AItem* InInventoryItem, int TopLeftIndex);

	bool TryAddInventoryItem(AItem* InInventoryItem);

	bool AddInventoryItem(AItem* InInventoryItem, int TopLeftIndex);

	AItem* GetItemAtIndex(int InIndex);

	bool RemoveInventoryItem(AItem* InInventoryItem);

	bool RemoveInventoryItemAtLocation(int32 Index);

	AItem* GetInventoryItemAtLocation(int32 Index);

	void ModifyInventoryItem(AItem* InventoryItem);

	void ModifyInventoryItemAtLocation(int32 Index);

	void CombineInventoryItems(AItem* InventoryItemA, AItem* InventoryItemB);

	void CombineInventoryItemsAtLocations(int32 IndexA, int32 IndexB);

	void SetToQuickSelect(AItem* InventoryItem);

	void RemoveFromQuickSelect(AItem* InventoryItem);

	UFUNCTION(BlueprintCallable)
	void RefreshInventoryWidget();

	UFUNCTION(BlueprintCallable)
	void CreateQuickSelectPieWidget(UPieMenu* InQuickSelectWidget);

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget(UInventoryWidget* InInventoryWidget);

	FORCEINLINE bool GetQuickSelectVisibility() const {
		return bQuickSelectVisible;
	}

	FORCEINLINE bool GetInventoryVisible() const {
		return bInventoryVisible;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPieMenu* GetQuickSelectPieWidget() const {
		return QuickSelectPieWidget;
	}
};
