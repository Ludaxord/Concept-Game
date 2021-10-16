#pragma once
UENUM(BlueprintType)
enum class EMagazineType : uint8 {
	EMT_None UMETA(DisplayName= "None Magazine"),
	EMT_AK47Mag UMETA(DisplayName= "AK47 Magazine"),
	EMT_M9Mag UMETA(DisplayName= "Beretta M9 Magazine"),
	EMT_AK74Mag UMETA(DisplayName= "AK74 Magazine"),
	EMT_AKValMag UMETA(DisplayName= "AKVal Magazine"),
	EMT_SMG11Mag UMETA(DisplayName= "SMG11 Magazine"),
	EMT_AR4Mag UMETA(DisplayName= "AR4 Magazine"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};
