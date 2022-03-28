// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIPerceptionComponent.h"
#include "BaseAIPercStimuliSourceComponent.h"
#include "TeamInfoInterface.h"
#include "BehaviorTreeAIController.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ABehaviorTreeAIController : public AAIController {
	GENERATED_BODY()

public:
	ABehaviorTreeAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* InActor, FAIStimulus InStimulus);

	UFUNCTION(BlueprintCallable)
	bool GetRandomLocationForActor(float InRadius, FVector& OutVector);

	UFUNCTION(BlueprintCallable)
	bool SetActorFocus(AActor* InFocusActor);

	UFUNCTION(BlueprintCallable)
	bool ClearActorFocus();

	bool IsVisibleActorEnemy(AActor* InTargetActor);

	bool ActorDetected(ITeamInfoInterface* CurrentTargetActor);

	AActor* GetClosestEnemy();

	void UpdateTargetActor();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* CurrentBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBaseAIPerceptionComponent* SightPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBaseAIPerceptionComponent* HearPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	AActor* LockActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	AActor* TargetActor;

	FTimerHandle TimerHandle;
};
