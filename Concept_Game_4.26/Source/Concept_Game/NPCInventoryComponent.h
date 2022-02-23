// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCInventoryComponent.generated.h"


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

	void InsertInventoryItem(class AItem* InItem);

	bool EquipItem(class AItem* InItem);

	AItem* GetItemOfClass(UClass* ItemClass);

	bool RemoveItemsOfClass(UClass* ItemClass);

	bool RemoveItemOfClass(UClass* ItemClass);

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
