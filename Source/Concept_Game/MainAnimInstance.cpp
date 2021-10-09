// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance(): CurrentAttackType(EAttackType::EAT_RightHandFireWeapon),
                                        ActionState(EActionState::EAS_Hip),
                                        bIsInAir(false),
                                        bIsAccelerating(false),
                                        bRunning(false),
                                        bAiming(false),
                                        bReloading(false),
                                        bTurnInPlace(false),
                                        bCrouching(false),
                                        bCrawling(false),
                                        MovementOffsetYaw(0.0f),
                                        LastMovementOffsetYaw(0.0f),
                                        RootYawOffset(0.0f),
                                        Speed(0.0f) {

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
