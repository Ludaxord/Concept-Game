// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MainCharacterCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AMainCharacterCameraManager : public APlayerCameraManager {
	GENERATED_BODY()
public:
	AMainCharacterCameraManager();

	virtual void DoUpdateCamera(float DeltaTime) override;
	virtual void UpdateCamera(float DeltaTime) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	FVector SwayIdle();
	FVector SwayAim();

private:
	class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	FRotator FinalCameraRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	FVector FinalCameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	float FinalCameraFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	FVector SwayOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	float SwayYawIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Properties", meta= (AllowPrivateAccess = "true"))
	float SwayPitchIntensity;


};
