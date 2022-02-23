// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionState.h"
#include "AttackType.h"
#include "PoseType.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UNPCAnimInstance : public UAnimInstance {
	GENERATED_BODY()

public:
	UNPCAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta= (AllowPrivateAccess = "true"))
	class ANPCBase* NPCCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAbleToAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat", meta= (AllowPrivateAccess = "true"))
	EAttackType CurrentAttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta = (AllowPrivateAccess = "true"))
	EActionState ActionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bItemEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bClimbing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bInCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bCoveringActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bMoveInCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float RootYawOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float YawInPlace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float LastYawInPlace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float YawDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place", meta= (AllowPrivateAccess = "true"))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta= (AllowPrivateAccess = "true"))
	float Speed;
};
