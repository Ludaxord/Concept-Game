// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIPerceptionComponent.h"
#include "GOAPAIController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCurrentNPCState, FString, State);

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AGOAPAIController : public AAIController {
	GENERATED_BODY()

	AGOAPAIController();

	UFUNCTION(BlueprintCallable)
	void Create(TArray<class UGOAPTaskComponent*> AITasks);

	UFUNCTION(BlueprintCallable)
	void CompleteTask();

	UFUNCTION(BlueprintCallable)
	void Update();

	UFUNCTION(BlueprintCallable)
	void InterruptTask();

	UFUNCTION(BlueprintCallable)
	void InitGoals(class ANPCBase* InPawn);

	virtual void OnPossess(APawn* InPawn) override;

private:
	void PlanTasks();

	void SetTasks();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<UGOAPTaskComponent*> Tasks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<class UGOAPGoalComponent*, int32> Goals;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	UGOAPTaskComponent* CurrentTask;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	UBaseAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	class AWorldStateManager* StateManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> States;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	class AGOAPPlanner* Planner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<UGOAPTaskComponent*> TasksQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP", meta = (AllowPrivateAccess = "true"))
	UGOAPGoalComponent* CurrentGoal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP", meta = (AllowPrivateAccess = "true"))
	bool bInterruptCurrentAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "GOAP",
		meta = (AllowPrivateAccess = "true"))
	FSetCurrentNPCState SetCurrentNPCStateDelegate;

	bool bInvoked = false;

	FTimerHandle OnUpdateTimer;
};
