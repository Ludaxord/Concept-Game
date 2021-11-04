#pragma once

UENUM(BlueprintType)
enum class EFireMode: uint8 {
	EFM_None UMETA(DisplayName = "None"),
	EFM_Single UMETA(DisplayName = "None"),
	EFM_Automatic UMETA(DisplayName = "Ait"),
	EFM_MAX UMETA(DisplayName = "DefaultMAX")
};
