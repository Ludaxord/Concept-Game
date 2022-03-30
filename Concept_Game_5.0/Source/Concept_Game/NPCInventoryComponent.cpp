// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInventoryComponent.h"

#include "Item.h"
#include "NPCBase.h"

// Sets default values for this component's properties
UNPCInventoryComponent::UNPCInventoryComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNPCInventoryComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<ANPCBase>(GetOwner());
}


// Called every frame
void UNPCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UNPCInventoryComponent::InsertInventoryItem(AItem* InItem) {
	if (InItem != nullptr) {
		InItem->SetItemState(EItemState::EIS_Equipped);
		InventoryItems.Add(InItem, false);
		return true;
	}

	return false;
}

bool UNPCInventoryComponent::EquipItem(AItem* InItem) {
	if (InItem == OwningCharacter->GetEquippedItem_Implementation()) {
		return false;
	}

	OwningCharacter->EquipItem_Implementation(InItem);
	return true;
}

AItem* UNPCInventoryComponent::GetItemOfClass(UClass* ItemClass) {
	for (TTuple<AItem*, bool> ItemPair : InventoryItems) {
		if (ItemPair.Key->IsA(ItemClass) && !ItemPair.Value) {
			ItemPair.Value = true;
			return ItemPair.Key;
		}
	}

	return nullptr;
}

bool UNPCInventoryComponent::RemoveItemsOfClass(UClass* ItemClass) {
	bool bRemoved = false;
	for (const TTuple<AItem*, bool> ItemPair : InventoryItems) {
		if (ItemPair.Key->IsA(ItemClass)) {
			InventoryItems.Remove(ItemPair.Key);
			bRemoved = true;
		}
	}

	return bRemoved;
}

bool UNPCInventoryComponent::RemoveItemOfClass(UClass* ItemClass) {
	for (const TTuple<AItem*, bool> ItemPair : InventoryItems) {
		if (ItemPair.Key->IsA(ItemClass)) {
			InventoryItems.Remove(ItemPair.Key);
			return true;
		}
	}

	return false;
}

AWeapon* UNPCInventoryComponent::EquipWeaponByType(EInventoryWeapon InInventoryWeapon) {

	AWeapon* NewWeapon = nullptr;

	TArray<AItem*> Weapons = GetItemsOfClass(AWeapon::StaticClass());

	for (AItem* Item : Weapons) {
		AWeapon* Weapon = Cast<AWeapon>(Item);
		switch (InInventoryWeapon) {
		case EInventoryWeapon::EIW_HighestAmmo:
			if (NewWeapon != nullptr) {
				if (NewWeapon->GetWeaponUsability() > Weapon->GetWeaponUsability()) {
					NewWeapon = Weapon;
				}
			}
			else {
				NewWeapon = Weapon;
			}
			break;
		case EInventoryWeapon::EIW_HighestDamage:
			break;
		case EInventoryWeapon::EIW_HighRange:
			break;
		case EInventoryWeapon::EIW_MiddleRange:
			break;
		case EInventoryWeapon::EIW_LowRange:
			break;
		case EInventoryWeapon::EIW_Melee:
			break;
		case EInventoryWeapon::EIW_Any:
			if (Weapons.Num() > 0) {
				NewWeapon = Cast<AWeapon>(Weapons[0]);
			}
			break;
		default: ;
		}
	}

	return NewWeapon;
}

TArray<AItem*> UNPCInventoryComponent::GetItemsOfClass(UClass* ItemClass) {
	TArray<AItem*> ItemsOfClass;
	for (TTuple<AItem*, bool> ItemPair : InventoryItems) {
		if (ItemPair.Key->IsA(ItemClass) && !ItemPair.Value) {
			ItemPair.Value = true;
			ItemsOfClass.Add(ItemPair.Key);
		}
	}

	return ItemsOfClass;
}
