// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "UObject/NoExportTypes.h"
#include "ItemContextObject.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryActionDelegate);

UCLASS(BlueprintType)
class CONCEPT_GAME_API UItemContextObject : public UObject {
	GENERATED_BODY()
public:
	UItemContextObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AItem* OwningItemReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Action Properties", meta = (AllowPrivateAccess = "true"))
	FText MenuActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Action Properties", meta = (AllowPrivateAccess = "true"))
	EContextMenuType ContextMenuType;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FInventoryActionDelegate MenuActionDelegate;
};
