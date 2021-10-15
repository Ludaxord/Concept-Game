#pragma once

#include "CoreMinimal.h"
#include "Effects.generated.h"

USTRUCT()
struct FEffect {
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct FCustomDepth : public FEffect {
	GENERATED_BODY()

	FCustomDepth();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLinearColor GlowColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLinearColor LightColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

	void EnableCustomDepth();
	void DisableCustomDepth();
};

USTRUCT(BlueprintType)
struct FGlowMaterial : public FEffect {
	GENERATED_BODY()

	FGlowMaterial();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* PulseCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere)
	float GlowAmount;

	UPROPERTY(VisibleAnywhere)
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere)
	float FresnelReflectFraction;

	void UpdatePulse();

	void ResetPulseTimer();
	void StartPulseTimer();
};
