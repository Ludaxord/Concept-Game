// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestsComponent.generated.h"


USTRUCT(BlueprintType)
struct FQuestStep {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Step", meta = (AllowPrivateAccess = "true"))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Step", meta = (AllowPrivateAccess = "true"))
	class AQuestTrigger* Trigger;
};

USTRUCT(BlueprintType)
struct FQuest {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FQuestStep> QuestSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	bool bPrimary;
};

USTRUCT(BlueprintType)
struct FNPCQuest {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FQuestStep> QuestSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	bool bPrimary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	bool bCanBeAccepted;
};

USTRUCT(BlueprintType)
struct FNPCQuestStore {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FNPCQuest> Quests;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddRemoveQuest);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UQuestsComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UQuestsComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddQuest(FNPCQuest InQuest);

	UFUNCTION(BlueprintCallable)
	void SelectQuest(FString InName);

	UFUNCTION(BlueprintCallable)
	void RemoveQuest();

	UFUNCTION(BlueprintCallable)
	void CompleteQuestStep();

	UFUNCTION(BlueprintCallable)
	virtual void QuestListToggle();

private:
	void ResetQuest();

	void UpdateCache();

private:
	friend class AQuestTrigger;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FQuest> Quests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Quest", meta = (AllowPrivateAccess = "true"))
	int32 CurrentQuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Quest", meta = (AllowPrivateAccess = "true"))
	FQuest ActiveQuest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Quest", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FQuest> QuestCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Quest", meta = (AllowPrivateAccess = "true"))
	bool bCanChangeQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* QuestListWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	bool bQuestListVisible;

public:
	UPROPERTY(BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAddRemoveQuest AddRemoveQuestDelegate;
};
