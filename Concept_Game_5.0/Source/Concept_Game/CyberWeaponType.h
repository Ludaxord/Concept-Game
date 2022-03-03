#pragma once
UENUM(BlueprintType)
enum class ECyberWeaponType : uint8 {
	ECWT_Hack UMETA(DisplayName = "Hack"),
	ECWT_Stealth UMETA(DisplayName = "Stealth"),
	ECWT_WireTravel UMETA(DisplayName = "Travel Through the wires"),
	ECWT_EnvScan UMETA(DisplayName = "Environment Scan"),
	ECWT_MAX UMETA(DisplayName = "DefaultMAX")
};