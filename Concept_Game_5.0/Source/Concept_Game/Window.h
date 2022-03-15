// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsBasedItem.h"
#include "Components/TimelineComponent.h"
#include "Window.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AWindow : public APhysicsBasedItem {
	GENERATED_BODY()

public:
	AWindow();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	virtual void SetWindowRotation(FRotator NewRotator, FVector NewLocation);
private:
	UFUNCTION()
	void UpdateWindowMovementTransitionTimeline(float Output);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Window Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Window Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsOpenedRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Window Properties", meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat WindowMovementFunctionFloat{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Window Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* WindowMovementTransitionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float CurrentWindowRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float RotationYawAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float RotationPitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float RotationRollAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float LocationXMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float LocationYMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Window Properties", meta = (AllowPrivateAccess = "true"))
	float LocationZMovement;

	UPROPERTY(EditAnywhere, Category="Window Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* WindowMovementTransitionFloatCurve;

	float CurrentRotYaw;

	float CurrentRotPitch;

	float CurrentRotRoll;

	float CurrentLocX;

	float CurrentLocY;

	float CurrentLocZ;

	FTimerHandle TimerHandle;
};
