#pragma once
UENUM(BlueprintType)
enum class EPoseType : uint8 {
	EPT_Stand UMETA(DisplayName = "Standing"),
	EPT_Crouch UMETA(DisplayName = "Crouching"),
	EPT_Run UMETA(DisplayName = "Running"),
	EPT_Aim UMETA(DisplayName = "Aiming"),
	EPT_Crawl UMETA(DisplayName = "Crawling"),
	EPT_Climb UMETA(DisplayName = "Climbing"),
	EPT_MAX UMETA(DisplayName = "DefaultMAX")
};
