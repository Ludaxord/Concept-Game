// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogReplyObject.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogReplyClicked);

UCLASS(BlueprintType)
class CONCEPT_GAME_API UDialogReplyObject : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialog", meta=(AllowPrivateAccess = "true", ExposeOnSpawn=true))
	FText Reply;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FOnDialogReplyClicked OnDialogReplyClickedDelegate;

};
