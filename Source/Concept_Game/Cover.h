// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "GameFramework/Actor.h"
#include "Cover.generated.h"

UCLASS()
class CONCEPT_GAME_API ACover : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetupCoverMeshSize();

	UFUNCTION(BlueprintCallable)
	void SetupCoverSize();

	UFUNCTION(BlueprintCallable)
	void SetupTakeCoverCollisionCapsule();

	void TakeCover();

	void QuitCover();

	void InCoverSystem();

	FVector MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Cover Properties", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CoverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cover Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TakeCoverCollisionBox;

	FGuid ID;

	AMainCharacter* CurrentOverlappingCharacter;

public:
	FORCEINLINE void SetCurrentOverlappingCharacter(AMainCharacter* InCurrentOverlappingCharacter) {
		CurrentOverlappingCharacter = InCurrentOverlappingCharacter;
	}
};
