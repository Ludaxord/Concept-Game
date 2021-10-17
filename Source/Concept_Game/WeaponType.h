#pragma once
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	EWT_Any UMETA(DisplayName = "Any Weapon"),
	EWT_Melee UMETA(DisplayName = "Melee Weapon"),
	EWT_Fire UMETA(DisplayName = "Fire Weapon"),
	EWT_Force UMETA(DisplayName = "Force Weapon"),
	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};
