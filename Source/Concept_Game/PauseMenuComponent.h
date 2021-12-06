// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PauseMenuComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UPauseMenuComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPauseMenuComponent();

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
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pause", meta = (AllowPrivateAccess = "true"))
	class UPauseMenuWidget* PauseMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pause", meta= (AllowPrivateAccess = "true"))
	bool bPauseMenuVisible;

public:
	UFUNCTION(BlueprintCallable)
	void CreatePauseMenu(UPauseMenuWidget* InPauseMenuWidget);

	UFUNCTION(BlueprintCallable)
	void PauseVisibilityState();

	UFUNCTION(BlueprintCallable)
	void PauseMenuToggle();

	void SetPauseMenuVisibility(bool bInVisibility) {
		bPauseMenuVisible = bInVisibility;
	}
};
