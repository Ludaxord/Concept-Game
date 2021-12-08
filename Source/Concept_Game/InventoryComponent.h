// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuickSelectInventoryVisibility, bool, Visible);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetQuickSelectMenu, bool, Setup);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UInventoryComponent : public UActorComponent, public IInventoryInterface {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void QuickSelectToggle(bool Visible);

	virtual void InventoryToggle();

	//TODO: Move to some base class to remove duplicates
	UFUNCTION(BlueprintCallable)
	FVector2D GetViewportCenter();

	UFUNCTION(BlueprintCallable)
	void QuickSelectInteractions();

	UFUNCTION(BlueprintCallable)
	float GetMouseRotationInViewport();

	UFUNCTION(BlueprintCallable)
	void SetQuickSelectArrowAngle(UUserWidget* ArrowWidget, float InAngle);

	UFUNCTION(BlueprintCallable)
	FIntPoint SetViewportSizeForQuickSelect();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(BlueprintAssignable, Category="Inventory Delegates", meta = (AllowPrivateAccess = "true"))
	FQuickSelectInventoryVisibility QuickSelectVisibilityDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory Delegates", meta = (AllowPrivateAccess = "true"))
	FSetQuickSelectMenu SetQuickSelectMenuDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSelect", meta = (AllowPrivateAccess = "true"))
	bool bQuickSelectVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSelect", meta = (AllowPrivateAccess = "true"))
	bool bQuickSelectVisibleRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta = (AllowPrivateAccess = "true"))
	bool bInventoryVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<class AItem*> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	FMatrix InventoryItemsPlacement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	TArray<class AItem*> QuickSelectItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	class UInventoryMenu* QuickSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	FIntPoint QuickSelectViewportSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "QuickSelect", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility CurrentQuickSelectWidgetVisibility;

public:
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
	void CreateQuickSelectWidget(UInventoryMenu* InQuickSelectWidget);

	FORCEINLINE bool GetQuickSelectVisibility() const {
		return bQuickSelectVisible;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryMenu* GetQuickSelectWidget() const {
		return QuickSelectWidget;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FIntPoint GetQuickSelectViewportSize() const {
		return QuickSelectViewportSize;
	}

	UFUNCTION(BlueprintCallable)
	void SetQuickSelectViewportSize(FIntPoint InQuickSelectViewportSize) {
		QuickSelectViewportSize = InQuickSelectViewportSize;
	}
};
