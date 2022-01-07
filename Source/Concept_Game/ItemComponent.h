// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void TraceForPhysicsItems();
	void TraceForItems();
	void TraceForLadder();

private:
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class AItem* CurrentInteractItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	//TODO: Remove it when implemented inside AItem

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class APhysicsInteractionItem* TraceHitPhysicsItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	class APhysicsInteractionItem* CurrentPhysicsItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	APhysicsInteractionItem* TraceHitPhysicsItemLastFrame;

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
};
