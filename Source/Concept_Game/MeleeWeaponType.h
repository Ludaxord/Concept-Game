#pragma once
UENUM(BlueprintType)
enum class EMeleeWeaponType : uint8 {
	EMWT_BaseballBat UMETA(DisplayName = "Baseball Bat"),
	EMWT_Wrench UMETA(DisplayName = "Wrench"),
	EMWT_Screwdriver UMETA(DisplayName = "Screwdriver"),
	EMWT_AutomaticScrewdriver UMETA(DisplayName = "Automatic Screwdriver"),
	EMWT_KitchenKnife UMETA(DisplayName = "Kitchen Knife"),
	EMWT_FuturaWhip UMETA(DisplayName = "Futura Whip"),
	EMWT_FuturaBlades UMETA(DisplayName = "Futura Blades"),
	EMWT_MAX UMETA(DisplayName = "DefaultMAX")
};
