// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "Item.h"
#include "Magazine.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API AMagazine : public AItem {
	GENERATED_BODY()

public:
	AMagazine();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ammo", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AmmoCollisionSphere;

public:
	FORCEINLINE UStaticMeshComponent* GetAmmoMesh() const {
		return AmmoMesh;
	}

	FORCEINLINE EAmmoType GetAmmoType() const {
		return AmmoType;
	}
};
