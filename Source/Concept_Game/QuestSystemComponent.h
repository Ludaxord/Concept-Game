// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestSystemComponent.generated.h"


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

	FNPCQuest(): bPrimary(true), bCanBeAccepted(true) {

	};

	bool operator==(const FNPCQuest& Other) const {

		return (Other.Name == Name);

	}

	bool operator==(const FNPCQuest&& Other) const {
		return (Other.Name == Name);
	}

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

	//TODO: Change TArray to TQueue, for easy removing items...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FNPCQuest> Quests;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddRemoveQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestListWidgetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeListQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuestInfoWidgetState, bool, bIsClosed, FString, InQuestName, FString,
                                               InQuestDescription);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAcceptQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAskForQuest, AActor*, InQuestHolderActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStepUpdate, bool, bShowAnimation, bool, bQuestIsSet);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestNotification, bool, bQuestCompleted, FString, QuestName);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API UQuestSystemComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddQuest(FNPCQuest InQuest);

	UFUNCTION(BlueprintCallable)
	void SelectQuest(FString InName);

	UFUNCTION(BlueprintCallable)
	void RemoveQuest(FString InName);

	UFUNCTION(BlueprintCallable)
	void CompleteQuestStep();

	UFUNCTION(BlueprintCallable)
	virtual void QuestListToggle();

	UFUNCTION(BlueprintCallable)
	virtual void TraceForQuestsHolders();

private:
	void ResetQuest();

	UFUNCTION()
	void UpdateCache();

	void LeaveCachedQuestActorTrace();

	UFUNCTION()
	void AskForQuest(AActor* InQuestHolder);

	UFUNCTION()
	void AcceptQuest();

	UFUNCTION()
	void QuestState();

private:
	friend class AQuestTrigger;

	class AMainCharacter* OwningCharacter;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* QuestInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* QuestTrackerContainerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* QuestUpdatePanelWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	bool bQuestListVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	bool bQuestWidgetActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget", meta = (AllowPrivateAccess = "true"))
	bool bQuestInfoVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	AActor* QuestActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	AActor* QuestActorLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	class AActor* QuestHolderActor;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAddRemoveQuest AddRemoveQuestDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FChangeListQuest ChangeListQuestDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAcceptQuest AcceptQuestDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FQuestState QuestStateDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAskForQuest AskForQuestDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FStepUpdate StepUpdateDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FQuestNotification QuestNotificationDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FQuestListWidgetState QuestListWidgetStateDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FQuestInfoWidgetState QuestInfoWidgetStateDelegate;

	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	FORCEINLINE bool GetQuestListVisibility() const {
		return bQuestListVisible;
	}

	FORCEINLINE bool GetQuestInfoVisibility() const {
		return bQuestInfoVisible;
	}

	FORCEINLINE int GetQuestID() const {
		return CurrentQuestID;
	}

	FORCEINLINE AActor* GetQuestActor() const {
		return QuestActor;
	}

	FORCEINLINE void SetQuestActor(AActor* InQuestActor) {
		QuestActor = InQuestActor;
	}

};
