// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AMainHUD : public AHUD {
	GENERATED_BODY()
public:
	AMainHUD();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character", meta =(AllowPrivateAccess = "true"))
	class ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D ScreenCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D CrossHairLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairSpreadMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairSpreadMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	float CrossHairHalfWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD", meta =(AllowPrivateAccess = "true"))
	FVector2D CrossHairBaseCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsTop;
};
