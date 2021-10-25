// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "Components/TimelineComponent.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ALadder : public AInteractiveItem {
	GENERATED_BODY()
public:
	ALadder();

	virtual void BeginPlay() override;

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	UPROPERTY(EditAnywhere, Category="Ladder Properties")
	UCurveFloat* LadderTransitionFloatCurve;

protected:
	UFUNCTION(BlueprintCallable)
	void SetupLadderMeshSize();

	UFUNCTION(BlueprintCallable)
	void SetupLadderRungs();

	UFUNCTION(BlueprintCallable)
	void SetupClimbLadderBoxCollision();

	UFUNCTION(BlueprintCallable)
	void SetupAreaCapsule();

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult) override;

	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

	void ReinitLadderSubComponents();

	void EnableClimbing();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LadderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* RootLadderMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ladder Properties", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* LadderCollisionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	int RungsNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	float SpaceBetweenRungs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ladder Properties", meta = (AllowPrivateAccess = "true"))
	FString LadderMeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ladder Properties", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* LadderTransitionTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ladder Properties", meta = (AllowPrivateAccess = "true"))
	USceneComponent* InteractSceneComponent;

	TArray<UStaticMeshComponent*> LadderRungsComponents;

	bool bTouchingLadder;

	bool bOnSphereOverlap;

	FOnTimelineFloat UpdateFunctionFloat;

	FVector ActorLocationBeforeTransition;

	FRotator ActorRotationBeforeTransition;

	UFUNCTION()
	void UpdateLadderTransitionTimeline(float Output);

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
