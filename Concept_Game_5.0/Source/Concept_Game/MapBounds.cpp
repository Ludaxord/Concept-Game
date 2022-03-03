// Fill out your copyright notice in the Description page of Project Settings.


#include "MapBounds.h"

#include "Components/BoxComponent.h"

// Sets default values
AMapBounds::AMapBounds() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MapBoundsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MapBoundsBox"));

}

// Called when the game starts or when spawned
void AMapBounds::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AMapBounds::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
