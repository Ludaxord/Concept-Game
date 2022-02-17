// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
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

	void AttachActors(UClass* ActorClass);

	bool FindNearestActorLocationFromOwner();


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool PrePerform();

	virtual bool PostPerform();

	virtual bool DuringPerform();

	virtual bool PerformAction();

	virtual bool IsViable();

	virtual bool GoalInterrupt(UGOAPTaskComponent* InCurrentGoal);

	virtual void CallActors();

	bool IsViableGiven(const TMap<FString, int32> InConditions);

	virtual bool StartAction();

protected:
	friend class AGOAPAIController;
	friend class AGOAPPlanner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	class ANPCBase* TaskOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> TaskAttachedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	AActor* NearestAttachedActor;

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
	float Range = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	bool bRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	bool bLoopTaskTillBreak = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	bool bIsViableTask = true;

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

	FORCEINLINE void SetViableTask(bool IsViableTask) {
		bIsViableTask = IsViableTask;
	}

	void SetAIController(class AAIController* InOwningAIController) {
		if (TaskOwner != nullptr) {
			TaskOwner->SetAIController(InOwningAIController);
		}
	}
};
