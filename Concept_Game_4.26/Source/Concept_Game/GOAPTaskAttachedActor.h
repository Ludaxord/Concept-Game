// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOAPTaskAttachedActor.generated.h"

//TODO: Change to Interface, to implement into NPC Actors also...
UCLASS()
class CONCEPT_GAME_API AGOAPTaskAttachedActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGOAPTaskAttachedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	bool bEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> DisabledActors;
};
