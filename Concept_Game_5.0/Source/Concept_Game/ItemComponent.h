// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UItemComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TraceForItems();
	void HoldCurrentItem();
	void TraceForLadder();

private:
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* CurrentInteractItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* TraceHitItemHitComponent;

	friend class AMainCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* PhysicsHandleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabHandleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	bool bIsHoldingItem;
public:
	FORCEINLINE AItem* GetTraceHitItem() const {
		return TraceHitItem;
	}

	FORCEINLINE AItem* GetCurrentInteractItem() const {
		return CurrentInteractItem;
	}

	FORCEINLINE AItem* GetTraceHitItemLastFrame() const {
		return TraceHitItemLastFrame;
	}

	FORCEINLINE void SetTraceHitItem(AItem* InItem) {
		TraceHitItem = InItem;
	}

	FORCEINLINE void SetCurrentInteractItem(AItem* InItem) {
		CurrentInteractItem = InItem;
	}

	FORCEINLINE void SetTraceHitItemLastFrame(AItem* InItem) {
		TraceHitItemLastFrame = InItem;
	}

	FORCEINLINE UPrimitiveComponent* GetTraceHitItemHitComponent() const {
		return TraceHitItemHitComponent;
	}

	FORCEINLINE UPhysicsHandleComponent* GetPhysicsHandleComponent() const {
		return PhysicsHandleComponent;
	}

	FORCEINLINE USceneComponent* GetGrabHandleComponent() const {
		return GrabHandleComponent;
	}

	bool IsHoldingItem();
};
