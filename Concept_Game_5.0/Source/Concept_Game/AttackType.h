#pragma once
UENUM(BlueprintType)
enum class EAttackType : uint8 {
	EAT_LeftHandFireWeapon UMETA(DisplayName = "Left Hand Fire Weapon"),
	EAT_RightHandFireWeapon UMETA(DisplayName = "Right Hand Fire Weapon"),
	EAT_LeftHandMeleeWeapon UMETA(DisplayName = "Left Hand Melee Weapon"),
	EAT_RightHandMeleeWeapon UMETA(DisplayName = "Right Hand Melee Weapon"),
	EAT_LeftHandForceWeapon UMETA(DisplayName = "Left Hand Force Weapon"),
	EAT_RightHandForceWeapon UMETA(DisplayName = "Right Hand Force Weapon"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};