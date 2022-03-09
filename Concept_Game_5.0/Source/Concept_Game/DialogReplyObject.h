// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogReplyObject.generated.h"

/**
 * 
 */


UCLASS(BlueprintType)
class CONCEPT_GAME_API UDialogReplyObject : public UObject {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogReplyClicked, UDialogReplyObject*, ClickedObject);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialog",
		meta=(AllowPrivateAccess = "true", ExposeOnSpawn=true))
	FText Reply;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FOnDialogReplyClicked OnDialogReplyClickedDelegate;

};
