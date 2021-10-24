// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ALadder : public AInteractiveItem {
	GENERATED_BODY()
public:
	ALadder();

protected:
	UFUNCTION(BlueprintCallable)
	void SetupLadderMeshSize();

	UFUNCTION(BlueprintCallable)
	void SetupLadderRungs();

	UFUNCTION(BlueprintCallable)
	void SetupClimbLadderBoxCollision();

	void ReinitLadderSubComponents();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LadderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* RootLadderMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ladder Properties", meta = (AllowPrivateAccess = "true"))
	USphereComponent* LadderCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	int RungsNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	float SpaceBetweenRungs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	FString LadderMeshName;

	TArray<UStaticMeshComponent*> LadderRungsComponents;

public:
	FORCEINLINE UInstancedStaticMeshComponent* GetRootLadderMeshComponent() const {
		return RootLadderMeshComponent;
	}

	FORCEINLINE UStaticMesh* GetLadderMesh() const {
		return LadderMesh;
	}

	FORCEINLINE int GetRungsNumber() const {
		return RungsNumber;
	}
};
