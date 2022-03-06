// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionComponent.h"
#include "DialogComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddRemoveDialogWidget, bool, bIsOpened);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogSpeakFinish);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAskForDialog, AActor*, InQuestHolderActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogReplyFinish, int, ReplyIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONCEPT_GAME_API UDialogComponent : public UActionComponent {
	GENERATED_BODY()
public:
	UDialogComponent();

	void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void TraceForDialogHolders();

	UFUNCTION()
	void AskForDialog(AActor* InQuestHolderActor);

private:
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Dialog", meta = (AllowPrivateAccess = "true"))
	AActor* DialogActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Dialog", meta = (AllowPrivateAccess = "true"))
	AActor* DialogActorLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest", meta = (AllowPrivateAccess = "true"))
	class AActor* DialogHolderActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* DialogWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	bool bDialogWidgetVisible;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAddRemoveDialogWidget AddRemoveDialogWidgetDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAskForDialog AskForDialogDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FDialogSpeakFinish DialogSpeakFinishDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FDialogReplyFinish DialogSpeakReplyDelegate;

	AActor* GetDialogActor() const {
		return DialogActor;
	}

	void SetDialogActor(AActor* InActor) {
		DialogActor = InActor;
	}
};
