#pragma once
UENUM(BlueprintType)
enum class EThrowableWeaponType : uint8 {
	ETWT_G67_Grenade UMETA(DisplayName = "G67 Grenade"),
	ETWT_M9_Knife UMETA(DisplayName = "M9 Knife"),
	ETWT_MAX UMETA(DisplayName = "DefaultMAX")
};
