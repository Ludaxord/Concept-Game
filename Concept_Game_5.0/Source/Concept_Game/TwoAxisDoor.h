// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "TwoAxisDoor.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ATwoAxisDoor : public ADoor {
	GENERATED_BODY()
public:
	ATwoAxisDoor();

	virtual void InteractWithItem(AMainCharacter* InCharacter) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult) override;

	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;

	virtual void OnBoxHandrailLeftBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                           bool bFromSweep,
	                                           const FHitResult& SweepResult);

	virtual void OnBoxHandrailLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                         UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	virtual void OnBoxHandrailRightBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                            bool bFromSweep,
	                                            const FHitResult& SweepResult);

	virtual void OnBoxHandrailRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalBasedMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionLeftBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionRightBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* DoorOpenAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* DoorCloseAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bStairsOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bStairsHandrailLeftOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Door Properties", meta = (AllowPrivateAccess = "true"))
	bool bStairsHandrailRightOverlap;
};
