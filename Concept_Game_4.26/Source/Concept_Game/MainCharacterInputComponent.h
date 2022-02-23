// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharacterInputComponent.generated.h"
// Te¿ inna sprawa fox engine zosta³ stworzony na zlecenie Kojimy, tutaj Kojima dosta³ ju¿ gotowy produkt i musia³ na nim dzia³aæ

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UMainCharacterInputComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMainCharacterInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	void StopJumping();
	void ClimbRight(float Value);
	void ClimbRightActionPressed();
	void ClimbRightActionReleased();
	void TurnRate(float Rate);
	void Turn(float Value);
	void LookUpAtRate(float Rate);
	void LookUp(float Value);
	void Cover();
	void AimingButtonPressed();
	void AimingButtonReleased();
	void RunningButtonPressed(float Value);
	void RunningButtonReleased();
	void ChangePoseButtonPressed(FKey Key);
	void ChangePoseAxisButtonPressed(float Value);
	void Jump();
	void UseWeaponButtonPressed();
	void UseWeaponButtonReleased();
	void ChangeDebugCamera();
	void DebugAddSkillPoints();
	void DebugInvisibility();
	void ChangeDebugTriggerRotationYaw();
	void TriggerFPSCounter();
	void InteractButtonPressed();
	void DebugToggleFog();
	void InventoryButtonPressed();
	void DebugWidgetButtonPressed();
	void PauseButtonPressed();
	void DebugDropItem();
	void QuickSelectButtonPressed(float Value);

	bool AimButtonPressedBlocked();
	bool AimButtonReleasedBlocked();
	bool UseWeaponPressedBlocked();
	bool UseWeaponReleasedBlocked();
	bool TurnRateBlocked(float Rate);
	bool TurnBlocked(float Value);
	bool LookUpAtRateBlocked(float Rate);
	bool LookUpBlocked(float Value);

protected:
	friend class AMainCharacter;

	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	AMainCharacter* OwningCharacter;
};
