#pragma once
UENUM(BlueprintType)
enum class EFireWeaponType : uint8 {
	EFWT_Beretta UMETA(DisplayName = "Beretta"),
	EFWT_Glock UMETA(DisplayName = "Glock"),
	EFWT_Colt UMETA(DisplayName = "Colt"),
	EFWT_AK47 UMETA(DisplayName = "AK47"),
	EFWT_UZI UMETA(DisplayName = "UZI"),
	EFWT_MP40 UMETA(DisplayName = "MP 40"),
	EFWT_FuturaBeretta UMETA(DisplayName = "Futura Beretta"),
	EFWT_FuturaGlock UMETA(DisplayName = "Futura Glock"),
	EFWT_FuturaColt UMETA(DisplayName = "Futura Colt"),
	EFWT_FuturaAK47 UMETA(DisplayName = "Futura AK47"),
	EFWT_FuturaUZI UMETA(DisplayName = "Futura UZI"),
	EFWT_FuturaMP40 UMETA(DisplayName = "Futura MP 40"),
	EFWT_MAX UMETA(DisplayName = "DefaultMAX")
};
