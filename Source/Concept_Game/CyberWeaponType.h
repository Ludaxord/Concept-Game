#pragma once
UENUM(BlueprintType)
enum class ECyberWeaponType : uint8 {
	EFWT_Hack UMETA(DisplayName = "Hack"),
	EFWT_Stealth UMETA(DisplayName = "Stealth"),
	EFWT_WireTravel UMETA(DisplayName = "Travel Through the wires"),
	EFWT_EnvScan UMETA(DisplayName = "Environment Scan"),
	EFWT_MAX UMETA(DisplayName = "DefaultMAX")
};