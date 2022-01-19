// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "Drawer.generated.h"

USTRUCT(BlueprintType)
struct FDrawerItem {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DrawerReference;

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	AItem* Item;

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	FTransform PositionInDrawer;
};

USTRUCT(BlueprintType)
struct FDrawerElement {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category= "Drawer", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DrawerMesh;

	UPROPERTY(EditAnywhere, Category= "Drawer", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DrawerBoxMesh;

	UPROPERTY(EditAnywhere, Category="Drawer", meta = (AllowPrivateAccess = "true"))
	TArray<FDrawerItem> DrawerItems;

	UPROPERTY(EditAnywhere, Category="Drawer", meta = (AllowPrivateAccess = "true"))
	FVector EmptyPlaceAtDrawer;

	UPROPERTY(EditAnywhere, Category="Drawer", meta = (AllowPrivateAccess = "true"))
	bool bIsOpened;

	UPROPERTY(EditAnywhere, Category="Drawer", meta = (AllowPrivateAccess = "true"))
	FVector DrawerLoc;

	UPROPERTY(EditAnywhere, Category="Drawer", meta = (AllowPrivateAccess = "true"))
	FVector DrawerRelativeLoc;
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ADrawer : public APhysicsBasedItem {
	GENERATED_BODY()

public:
	ADrawer();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void FindTracingComponent();

	void UpdateDrawerMovement();

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	UFUNCTION()
	void UpdateDrawerMovementTransitionTimeline(float Output);

	UFUNCTION()
	void DrawerItemInteraction(AItem* InItem);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TopDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BottomDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MiddleDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TopDrawerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TopDrawerLeftPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TopDrawerRightPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TopDrawerBackPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TopDrawerFrontPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BottomDrawerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BottomDrawerLeftPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BottomDrawerRightPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BottomDrawerBackPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BottomDrawerFrontPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MiddleDrawerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MiddleDrawerLeftPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MiddleDrawerRightPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MiddleDrawerBackPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MiddleDrawerFrontPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CurrentTracingDrawerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowForwardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	TArray<FDrawerElement> Drawers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drawer Properties", meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> InsideItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drawer Properties", meta = (AllowPrivateAccess = "true"))
	TMap<UBoxComponent*, UStaticMeshComponent*> DrawerMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drawer Properties", meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat DrawerMovementFunctionFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drawer Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* DrawerMovementTransitionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	float CurrentDrawerMovement;

	UPROPERTY(EditAnywhere, Category="Drawer Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* DrawerMovementTransitionFloatCurve;

	bool bMoveDrawer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Drawer Properties", meta = (AllowPrivateAccess = "true"))
	int32 CurrentDrawerIndex;
};
