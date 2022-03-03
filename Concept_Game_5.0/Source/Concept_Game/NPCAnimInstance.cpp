// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAnimInstance.h"

#include "ActionState.h"
#include "NPCBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UNPCAnimInstance::UNPCAnimInstance() {
}

void UNPCAnimInstance::UpdateAnimationProperties(float DeltaTime) {
	if (NPCCharacter == nullptr)
		NPCCharacter = Cast<ANPCBase>(TryGetPawnOwner());

	if (NPCCharacter) {
		bIsInAir = NPCCharacter->GetCharacterMovement()->IsFalling();
		if (NPCCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0) {
			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}
		if (NPCCharacter->GetVelocity().Size() > 0) {
			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}


		FRotator AimRotation = NPCCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(NPCCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (NPCCharacter->GetVelocity().Size() > 0) {
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = NPCCharacter->GetAiming_Implementation();

		if (bReloading) {
			ActionState = EActionState::EAS_Reloading;
		}
		else if (bIsInAir) {
			ActionState = EActionState::EAS_InAir;
		}
		else if (NPCCharacter->GetAiming_Implementation()) {
			ActionState = EActionState::EAS_Aiming;
		}
		else if (bClimbing) {
			ActionState = EActionState::EAS_Climbing;
		}
		else {
			ActionState = EActionState::EAS_Hip;
		}

		bIsAbleToAttack = NPCCharacter->IsAbleToAttack_Implementation();

		if (bIsAbleToAttack) {
			CurrentAttackType = NPCCharacter->GetCurrentAttackType_Implementation();
		}

		PoseType = NPCCharacter->GetCurrentPoseType_Implementation();
		bReloading = NPCCharacter->GetCombatState_Implementation() == ECombatState::ECS_Reloading;
		bEquipping = NPCCharacter->GetCombatState_Implementation() == ECombatState::ECS_Equipping;
		bClimbing = NPCCharacter->GetCurrentPoseType_Implementation() == EPoseType::EPT_Climb;
		bItemEquipped = NPCCharacter->GetEquippedItem_Implementation() != nullptr;
		bInCover = NPCCharacter->GetInCover_Implementation();
		bCoveringActive = NPCCharacter->GetCoveringActive_Implementation();
		bMoveInCover = NPCCharacter->GetMoveInCover_Implementation();

		bCrawling = NPCCharacter->GetCurrentPoseType_Implementation() == EPoseType::EPT_Crawl;
		bCrouching = NPCCharacter->GetCurrentPoseType_Implementation() == EPoseType::EPT_Climb;
		bRunning = NPCCharacter->GetRunning_Implementation();

		FVector Velocity = NPCCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		if (bReloading) {
			ActionState = EActionState::EAS_Reloading;
		}
		else if (bIsInAir) {
			ActionState = EActionState::EAS_InAir;
		}
		else if (NPCCharacter->GetAiming_Implementation()) {
			ActionState = EActionState::EAS_Aiming;
		}
		else if (bClimbing) {
			ActionState = EActionState::EAS_Climbing;
		}
		else {
			ActionState = EActionState::EAS_Hip;
		}

		//TODO: Add Equipped Weapon Type
		if (NPCCharacter->GetEquippedItem_Implementation()) {

		}
	}
}

void UNPCAnimInstance::NativeInitializeAnimation() {
	NPCCharacter = Cast<ANPCBase>(TryGetPawnOwner());
}
