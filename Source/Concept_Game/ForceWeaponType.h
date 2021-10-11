#pragma once
UENUM(BlueprintType)
enum class EForceWeaponType : uint8 {
	EFWT_Hack UMETA(DisplayName = "Hack"),
	EFWT_Stealth UMETA(DisplayName = "Stealth"),
	EFWT_MAX UMETA(DisplayName = "DefaultMAX")
};