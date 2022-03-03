// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverPoint.generated.h"

UCLASS()
class CONCEPT_GAME_API ACoverPoint : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoverPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Point Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Point Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* MovementAreaSphere;

	ACharacter* AttachedCharacter;

	FGuid ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Point Properties", meta = (AllowPrivateAccess = "true"))
	class ACover* OverlappingCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Point Properties", meta = (AllowPrivateAccess = "true"))
	FVector CoverNormal;

public:
	FORCEINLINE void AttachCharacter(ACharacter* Character) {
		AttachedCharacter = Character;
	}

	FORCEINLINE ACover* GetOverlappingCover() {
		return OverlappingCover;
	}

	FORCEINLINE void SetCoverNormal(FVector Normal) {
		CoverNormal = Normal;
	}

	FORCEINLINE FVector GetCoverNormal() {
		return CoverNormal ;
	}
};
