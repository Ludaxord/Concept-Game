// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameStateBase.h"

#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"

void AMainGameStateBase::BeginPlay() {
	Super::BeginPlay();
}

void AMainGameStateBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AMainGameStateBase::SpawnItemFromActor(AActor* InActor, AItem* InItem, bool bGroundClamp) {
	FVector SpawnLocation = InActor->GetActorLocation() + InActor->GetActorForwardVector() * 150.0f;
	if (bGroundClamp) {
		TArray<AActor*> IgnoredActors;
		FHitResult HitResult;
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
		bool bHit = UKismetSystemLibrary::LineTraceSingle(this,
		                                                  SpawnLocation,
		                                                  SpawnLocation - FVector(0.0f, 0.0f, 1000.0f),
		                                                  TraceType,
		                                                  false,
		                                                  IgnoredActors,
		                                                  EDrawDebugTrace::ForOneFrame,
		                                                  HitResult,
		                                                  true,
		                                                  FLinearColor::MakeRandomColor(),
		                                                  FLinearColor::MakeRandomColor());
		if (bHit || HitResult.bBlockingHit) {
			SpawnLocation = HitResult.Location;
		}
	}

	// GetWorld()->SpawnActor()
}
