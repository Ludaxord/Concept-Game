// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "GameFramework/Pawn.h"
#include "NPCInterface.h"
#include "QuestHolderInterface.h"
#include "NPCBase.generated.h"

UCLASS()
class CONCEPT_GAME_API ANPCBase : public APawn, public INPCInterface, public IQuestHolderInterface {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact_Implementation(AMainCharacter* InCharacter) override;

	virtual void QuestInteract_Implementation(AMainCharacter* InCharacter) override;

	virtual bool QuestAvailable_Implementation() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                 bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC Mesh", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* NPCCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* NPCMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC Mesh", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NPCSphere;

	FGuid ID;

public:
	FORCEINLINE USkeletalMeshComponent* GetNPCMesh() const {
		return NPCMesh;
	}

	FORCEINLINE UCapsuleComponent* GetNPCCapsule() const {
		return NPCCapsule;
	}

	FORCEINLINE USphereComponent* GetNPCSphere() const {
		return NPCSphere;
	}
};
