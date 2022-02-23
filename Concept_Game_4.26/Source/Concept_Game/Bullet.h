// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoType.h"
#include "Item.h"
#include "Bullet.generated.h"

UENUM(BlueprintType)
enum class EBulletType : uint8 {
	EBT_Lethal UMETA(DisplayName = "Lethal"),
	EBT_Empty UMETA(DisplayName = "Empty"),
	EBT_Pickable UMETA(DisplayName = "Pickable"),
	EBT_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class CONCEPT_GAME_API ABullet : public AItem {
	GENERATED_BODY()
public:
	ABullet();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ammo", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ammo", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LethalBulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ammo", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* EmptyBulletMesh;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Ammo", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* PickableBulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AmmoCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	EBulletType BulletType;

public:
	FORCEINLINE UStaticMeshComponent* GetAmmoMesh() const {
		return AmmoMesh;
	}

	FORCEINLINE EAmmoType GetAmmoType() const {
		return AmmoType;
	}
};
