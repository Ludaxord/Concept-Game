// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldStateManager.generated.h"

//TODO Change AActor to UObject...
UCLASS()
class CONCEPT_GAME_API AWorldStateManager : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldStateManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool HasState(FString Key);

	UFUNCTION(BlueprintCallable)
	void AddState(FString Key, int32 Val);

	UFUNCTION(BlueprintCallable)
	void RemoveState(FString Key);

	UFUNCTION(BlueprintCallable)
	void SetState(FString Key, int32 Val);

	UFUNCTION(BlueprintCallable)
	void ModifyState(FString Key, int32 Val);

	UFUNCTION(BlueprintCallable)
	TMap<FString, int32> GetStates();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> States;
};
