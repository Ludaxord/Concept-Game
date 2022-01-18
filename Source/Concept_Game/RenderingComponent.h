// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	FORCEINLINE bool ApplyFog() {
		return bApplyFog;
	}

	FORCEINLINE void ApplyFog(bool bInApplyFog) {
		bApplyFog = bInApplyFog;
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
};
