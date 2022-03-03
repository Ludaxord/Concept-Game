// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItem.h"

#include "MainCharacter.h"

void AInteractiveItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult) {
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep,
	                            SweepResult);
	// if (OtherActor) {
	// 	AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
	// 	if (OtherCharacter != nullptr) {
	// 		UE_LOG(LogTemp, Warning, TEXT("AInteractiveItem %s Overlapped Component %s"), *GetName(),
	// 		       *OverlappedComponent->GetName());
	// 	}
	// }
}

void AInteractiveItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
}
