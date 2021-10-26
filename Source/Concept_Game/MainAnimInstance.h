// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AttackType.h"
#include "PoseType.h"
#include "MainAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EActionState: uint8 {
	EAS_Aiming UMETA(DisplayName = "Aiming"),
	EAS_Climbing UMETA(DisplayName = "Climbing"),
	EAS_Hip UMETA(DisplayName = "Hip"),
	EAS_Reloading UMETA(DisplayName = "Reloading"),
	EAS_InAir UMETA(DisplayName = "InAir"),
	EAS_MAX UMETA(DisplayName = "DefaultMAX"),
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UMainAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	UMainAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:
	void TurnInPlace();

	void Lean(float DeltaTime);

public:
	FORCEINLINE EAttackType GetCurrentAttackType() const {
		return CurrentAttackType;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Movement", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* MainCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat", meta= (AllowPrivateAccess = "true"))
	EAttackType CurrentAttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta = (AllowPrivateAccess = "true"))
	EActionState ActionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	bool bClimbingLadderFromBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta= (AllowPrivateAccess = "true"))
	bool bIsAimingAvailable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bClimbing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta = (AllowPrivateAccess = "true"))
	bool bTurnInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Pose", meta = (AllowPrivateAccess = "true"))
	EPoseType PoseType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Crouching", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Crawling", meta = (AllowPrivateAccess = "true"))
	bool bCrawling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float RootYawOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta= (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float RecoilWeight;

	float RotationCurve;

	float RotationCurveValueLastFrame;

	float TIPCharacterYaw;

	float TIPCharacterYawLastFrame;

	FRotator CharacterRotation;

	FRotator CharacterRotationLastFrame;
};
