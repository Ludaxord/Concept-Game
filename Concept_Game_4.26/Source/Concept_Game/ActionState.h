#pragma once
UENUM(BlueprintType)
enum class EActionState: uint8 {
	EAS_Aiming UMETA(DisplayName = "Aiming"),
	EAS_Climbing UMETA(DisplayName = "Climbing"),
	EAS_Hip UMETA(DisplayName = "Hip"),
	EAS_Reloading UMETA(DisplayName = "Reloading"),
	EAS_InAir UMETA(DisplayName = "InAir"),
	EAS_MAX UMETA(DisplayName = "DefaultMAX"),
};
