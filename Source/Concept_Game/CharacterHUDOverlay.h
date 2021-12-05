// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUDOverlay.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API UCharacterHUDOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ToggleVisibilityOfQuickSelect();

	UFUNCTION(BlueprintCallable)
	bool ToggleVisibilityOfPlayerHUD();

	void CastCharacterComponents();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reference", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reference", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* CharacterInventoryComponent;
};
