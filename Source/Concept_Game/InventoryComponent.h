// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuickSelectInventoryVisibility, bool, Visible);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetQuickSelectMenu, bool, Setup);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UInventoryComponent : public UActorComponent, public IInventoryInterface {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(BlueprintAssignable, Category="Inventory Delegates", meta = (AllowPrivateAccess = "true"))
	FQuickSelectInventoryVisibility QuickSelectVisibilityDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory Delegates", meta = (AllowPrivateAccess = "true"))
	FSetQuickSelectMenu SetQuickSelectMenuDelegate;
};
