// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance(): Speed(0.0f),
                                        bIsInAir(false),
                                        bIsAccelerating(false),
                                        bRunning(false),
                                        MovementOffsetYaw(0.0f),
                                        LastMovementOffsetYaw(0.0f),
                                        bAiming(false),
                                        RootYawOffset(0.0f),
                                        bReloading(false),
                                        ActionState(EActionState::EAS_Hip),
                                        bTurnInPlace(false),
                                        CurrentAttackType(EAttackType::EAT_RightHandFireWeapon) {

}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime) {
	if (MainCharacter == nullptr)
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::NativeInitializeAnimation() {
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::TurnInPlace() {
	if (MainCharacter == nullptr) return;
}

void UMainAnimInstance::Lean(float DeltaTime) {
	if (MainCharacter == nullptr) return;
}
