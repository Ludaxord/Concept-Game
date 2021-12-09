// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenuItem.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UInventoryMenuItem : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FText GetAmmo();

	UFUNCTION(BlueprintCallable)
	FText GetWeaponName();

	UFUNCTION(BlueprintCallable)
	FText GetWeaponType();

	UFUNCTION(BlueprintCallable)
	float GetWeaponDamagePercent();

	void CastCharacterComponents();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reference", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reference", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* CharacterInventoryComponent;
};