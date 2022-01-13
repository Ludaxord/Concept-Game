// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "Components/TimelineComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Cabinet.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FShelfItem {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ShelfReference;

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	AItem* ShelfItem;

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	FTransform PositionInShelf;
};

USTRUCT(BlueprintType)
struct FCabinetShelf {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ShelfReference;

	UPROPERTY(EditAnywhere, Category="Shelf", meta = (AllowPrivateAccess = "true"))
	TArray<FShelfItem> ShelfItems;
};

UCLASS()
class CONCEPT_GAME_API ACabinet : public APhysicsBasedItem {
	GENERATED_BODY()
public:
	ACabinet();

	virtual void BeginPlay() override;

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	virtual void Tick(float DeltaSeconds) override;
private:
	UFUNCTION()
	void UpdateDoorMovementTransitionTimeline(float Output);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* LeftDoorPhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* RightDoorPhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpenedRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat DoorMovementFunctionFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* DoorMovementTransitionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	float CurrentDoorRotation;

	UPROPERTY(EditAnywhere, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* DoorMovementTransitionFloatCurve;

	float CurrentLeftRotYaw;

	float CurrentRightRotYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> InsideItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	TArray<FCabinetShelf> Shelves;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ShelfPosition1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ShelfPosition2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WallLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WallRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cabinet Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WallBack;

	FTimerHandle TimerHandle;
};
