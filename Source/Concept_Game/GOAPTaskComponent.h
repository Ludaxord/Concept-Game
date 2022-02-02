// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOAPTaskComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UGOAPTaskComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGOAPTaskComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool PrePerform();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool PostPerform();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsViable();

	bool IsViableGiven(const TMap<FString, int32> InConditions);

private:
	friend class AGOAPAIController;
	friend class AGOAPPlanner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	FString TaskName = "Task";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	float Cost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	FVector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> PreConditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	float Range = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	bool bRunning = false;

public:
	FORCEINLINE FVector GetTarget() const {
		return Target;
	}

	FORCEINLINE FString GetTaskName() const {
		return TaskName;
	}

	FORCEINLINE float GetCost() const {
		return Cost;
	}

	FORCEINLINE float GetDuration() const {
		return Duration;
	}

	FORCEINLINE float GetRange() const {
		return Range;
	}
};
