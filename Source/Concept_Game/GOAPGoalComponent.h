// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOAPGoalComponent.generated.h"

UENUM(BlueprintType)
enum class EGOAPGoalType: uint8 {
	EGGT_Regular UMETA(DisplayName= "Regular Goal"),
	EGGT_Interrupt UMETA(DisplayName= "Interrupt Goal"),
	EGGT_WorldEvent UMETA(DisplayName= "World Event Goal"),
	EGGT_MAX UMETA(DisplayName = "MAX")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UGOAPGoalComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGOAPGoalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	friend class AGOAPAIController;
	friend class ANPCBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> Goals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	EGOAPGoalType GoalType;

	bool bRemove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	bool bSetAsCurrentGoal;
};
