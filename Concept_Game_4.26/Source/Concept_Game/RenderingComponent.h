// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "RenderingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API URenderingComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URenderingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleInvisibility();

	void UpdateInvisibilityTransitionTimeline(float Output);

	void PrepareInvisibility();

	FORCEINLINE bool ApplyFog() {
		return bApplyFog;
	}

	FORCEINLINE void ApplyFog(bool bInApplyFog) {
		bApplyFog = bInApplyFog;
	}

	FORCEINLINE bool ApplyInvisibility() {
		return bApplyInvisibility;
	}

	FORCEINLINE void ApplyInvisibility(bool bInApplyInvisibility) {
		bApplyInvisibility = bInApplyInvisibility;
	}

	FORCEINLINE bool ApplySnow() {
		return bApplySnow;
	}

	FORCEINLINE void ApplySnow(bool bInApplySnow) {
		bApplySnow = bInApplySnow;
	}

	FORCEINLINE bool ApplyRain() {
		return bApplyRain;
	}

	FORCEINLINE void ApplyRain(bool bInApplyRain) {
		bApplyRain = bInApplyRain;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fog Properties", meta = (AllowPrivateAccess = "true"))
	bool bApplyFog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fog Properties", meta = (AllowPrivateAccess = "true"))
	bool bApplySnow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fog Properties", meta = (AllowPrivateAccess = "true"))
	bool bApplyRain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Invisibility Properties", meta = (AllowPrivateAccess = "true"))
	bool bApplyInvisibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Invisibility Properties", meta = (AllowPrivateAccess = "true"))
	bool bInvisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* InvisibilityTransitionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	FOnTimelineFloat InvisibilityFunctionFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	UCurveFloat* InvisibilityTransitionFloatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* HeadMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Invisibility Properties",
		meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* SignMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Global Properties", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;
};
