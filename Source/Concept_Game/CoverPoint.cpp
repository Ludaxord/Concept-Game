// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverPoint.h"

#include "Cover.h"
#include "Components/SphereComponent.h"

// Sets default values
ACoverPoint::ACoverPoint() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMesh"));
	SetRootComponent(PointMesh);

	MovementAreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MovementAreaSphere"));
	MovementAreaSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACoverPoint::BeginPlay() {
	Super::BeginPlay();

	ID = FGuid::NewGuid();

	MovementAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoverPoint::OnSphereBeginOverlap);
	MovementAreaSphere->OnComponentEndOverlap.AddDynamic(this, &ACoverPoint::OnSphereEndOverlap);
	PointMesh->SetVisibility(false);
}

// Called every frame
void ACoverPoint::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACoverPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult) {
	ACover* CurrentOverlappingCover = Cast<ACover>(OtherActor);
	if (AttachedCharacter) {
		AMainCharacter* Character = Cast<AMainCharacter>(AttachedCharacter);
		if (Character) {
			if (!Character->GetSlideToLeftCover() && !Character->GetSlideToRightCover()) {
				ACover* CurrentCover = Character->GetCurrentCover();
				if (CurrentOverlappingCover != CurrentCover && Cast<AMainCharacter>(OtherActor) != AttachedCharacter) {
					PointMesh->SetVisibility(true);
					UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
					       *OtherActor->GetName(),
					       *OverlappedComponent->GetName(), *OtherComponent->GetName());
					OverlappingCover = CurrentOverlappingCover;
				}
			}
		}
	}
}

void ACoverPoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	ACover* CurrentOverlappingCover = Cast<ACover>(OtherActor);
	if (OverlappingCover == CurrentOverlappingCover) {
		PointMesh->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("End Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
		       *OtherActor->GetName(),
		       *OverlappedComponent->GetName(), *OtherComponent->GetName());
		OverlappingCover = nullptr;
	}
}
