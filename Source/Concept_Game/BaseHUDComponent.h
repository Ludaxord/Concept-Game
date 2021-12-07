// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHUDComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UBaseHUDComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseHUDComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//TODO: Move to some base class to remove duplicates
	UFUNCTION(BlueprintCallable)
	FVector2D GetViewportCenter();

	UFUNCTION(BlueprintCallable)
	void GetMouseRotationInViewport();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD", meta = (AllowPrivateAccess = "true"))
	class UBasePlayerHUDWidget* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HUD", meta= (AllowPrivateAccess = "true"))
	bool bBaseHUDVisible;

public:
	UFUNCTION(BlueprintCallable)
	void CreateBasePlayerHUD(UBasePlayerHUDWidget* InPlayerHUDWidget);

	UFUNCTION(BlueprintCallable)
	void HUDVisibilityState();

	void SetBaseHUDVisibility(bool bInVisibility) {
		bBaseHUDVisible = bInVisibility;
	}
};
