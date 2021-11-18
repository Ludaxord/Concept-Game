// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverPoint.h"

// Sets default values
ACoverPoint::ACoverPoint() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMesh"));
	SetRootComponent(PointMesh);
}

// Called when the game starts or when spawned
void ACoverPoint::BeginPlay() {
	Super::BeginPlay();

	ID = FGuid::NewGuid();
}

// Called every frame
void ACoverPoint::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
