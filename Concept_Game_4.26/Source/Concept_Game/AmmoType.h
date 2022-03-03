#pragma once
UENUM(BlueprintType)
enum class EAmmoType : uint8 {
	EAT_12Gauge UMETA(DisplayName= "12 Gauge"),
	EAT_40mm UMETA(DisplayName= "40mm"),
	EAT_45ap UMETA(DisplayName= "45ap"),
	EAT_545x39 UMETA(DisplayName= "545x39"),
	EAT_556x45 UMETA(DisplayName= "556x45"),
	EAT_762x39 UMETA(DisplayName= "762x39"),
	EAT_9x18 UMETA(DisplayName= "9x18"),
	EAT_9mm UMETA(DisplayName= "9mm"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};
