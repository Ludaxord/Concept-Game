// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionComponent.h"
#include "DialogComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddRemoveDialogWidget);

UCLASS()
class CONCEPT_GAME_API UDialogComponent : public UActionComponent {
	GENERATED_BODY()
public:
	UDialogComponent();

	void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void TraceForDialogHolders();

private:
	class AMainCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Dialog", meta = (AllowPrivateAccess = "true"))
	AActor* DialogActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Dialog", meta = (AllowPrivateAccess = "true"))
	AActor* DialogActorLastFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* DialogWidget;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FAddRemoveDialogWidget AddRemoveDialogWidgetDelegate;
public:
	AActor* GetDialogActor() const {
		return DialogActor;
	}

	void SetDialogActor(AActor* InActor) {
		DialogActor = InActor;
	}
};
