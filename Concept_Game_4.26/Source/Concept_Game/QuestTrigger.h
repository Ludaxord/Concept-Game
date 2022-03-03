// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestHolderInterface.h"
#include "QuestSystemComponent.h"
#include "GameFramework/Actor.h"
#include "QuestTrigger.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOverlapOn,UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComponent, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult)

USTRUCT(BlueprintType)
struct FQuestTriggerProperties {
	GENERATED_BODY()

	bool bSphereFinishStep;
};

UCLASS()
class CONCEPT_GAME_API AQuestTrigger : public AActor, public IQuestHolderInterface {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestTrigger();

	bool FinishStep();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void QuestInteract_Implementation(AMainCharacter* InCharacter) override;

	virtual bool QuestAvailable_Implementation() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                 bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

private:
	friend class ANPCQuestCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quests", meta = (AllowPrivateAccess = "true"))
	UQuestSystemComponent* QuestComponentRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quests", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* QuestTriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quests", meta = (AllowPrivateAccess = "true"))
	FString QuestName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quests", meta = (AllowPrivateAccess = "true"))
	FQuestStep QuestStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quests", meta = (AllowPrivateAccess = "true"))
	FQuestTriggerProperties QuestTriggerProperties;

	FGuid ID;
};
