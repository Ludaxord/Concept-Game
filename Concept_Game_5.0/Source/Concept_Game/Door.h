// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ADoor : public APhysicsBasedItem {
	GENERATED_BODY()

public:
	ADoor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult) override;

	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;
private:
	UFUNCTION()
	void UpdateDoorMovementTransitionTimeline(float Output);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpenedRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door Properties", meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat DoorMovementFunctionFloat{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* DoorMovementTransitionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	float CurrentDoorRotation;

	UPROPERTY(EditAnywhere, Category="Door Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* DoorMovementTransitionFloatCurve;

	float CurrentRotYaw;

	FTimerHandle TimerHandle;
};
