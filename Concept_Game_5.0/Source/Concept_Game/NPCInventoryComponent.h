// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCInventoryComponent.generated.h"


UENUM()
enum class EInventoryWeapon : uint8 {
	EIW_HighestAmmo UMETA(DisplayName = "Highest Ammo"),
	EIW_HighestDamage UMETA(DisplayName = "Highest Damage"),
	EIW_HighRange UMETA(DisplayName = "High Range"),
	EIW_MiddleRange UMETA(DisplayName = "Middle Range"),
	EIW_LowRange UMETA(DisplayName = "Low Range"),
	EIW_Melee UMETA(DisplayName = "Melee"),
	EIW_Any UMETA(DisplayName = "Any"),
	EIW_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UNPCInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNPCInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool InsertInventoryItem(class AItem* InItem);

	bool EquipItem(class AItem* InItem);

	AItem* GetItemOfClass(UClass* ItemClass);

	bool RemoveItemsOfClass(UClass* ItemClass);

	bool RemoveItemOfClass(UClass* ItemClass);

	class AWeapon* EquipWeaponByType(EInventoryWeapon InInventoryWeapon);

	TArray<AItem*> GetItemsOfClass(UClass* ItemClass);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class ANPCBase* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<class AItem*, bool> InventoryItems;

public:
	FORCEINLINE TMap<class AItem*, bool> GetInventoryItems() {
		return InventoryItems;
	}
};
