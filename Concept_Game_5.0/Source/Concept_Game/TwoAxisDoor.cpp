// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoAxisDoor.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATwoAxisDoor::ATwoAxisDoor(): ADoor() {
	ItemInteractionName = "Open";
	ItemName = "Door";

	SkeletalBasedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalBasedMesh"));

	CollisionLeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionLeftBox"));
	CollisionRightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionRightBox"));

	SetRootComponent(SkeletalBasedMesh);

	SkeletalBasedMesh->SetSimulatePhysics(false);
	SkeletalBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	AreaSphere->SetupAttachment(SkeletalBasedMesh);
	PickupWidget->SetupAttachment(SkeletalBasedMesh);
	CollisionBox->SetupAttachment(SkeletalBasedMesh);

	CollisionLeftBox->SetupAttachment(SkeletalBasedMesh);
	CollisionRightBox->SetupAttachment(SkeletalBasedMesh);

	PhysicsBasedMesh->SetupAttachment(nullptr);
	PhysicsBasedMesh = nullptr;
}

void ATwoAxisDoor::InteractWithItem(AMainCharacter* InCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("ATwoAxisDoor::InteractWithItem"))
	bIsOpened = !bIsOpened;
	SkeletalBasedMesh->PlayAnimation(bIsOpened ? DoorOpenAnimation : DoorCloseAnimation, false);
	ItemInteractionName = bIsOpened ? "Close" : "Open";
}

void ATwoAxisDoor::BeginPlay() {
	Super::BeginPlay();

	// CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxBeginOverlap);
	// CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxEndOverlap);

	// CollisionLeftBox->OnComponentBeginOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxHandrailLeftBeginOverlap);
	// CollisionLeftBox->OnComponentEndOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxHandrailLeftEndOverlap);

	// CollisionRightBox->OnComponentBeginOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxHandrailRightBeginOverlap);
	// CollisionRightBox->OnComponentEndOverlap.AddDynamic(this, &ATwoAxisDoor::OnBoxHandrailRightEndOverlap);

	bIsOpened = true;
}

void ATwoAxisDoor::Tick(float DeltaSeconds) {
	// UE_LOG(LogTemp, Warning,
	//        TEXT("bStairsOverlap %s && bStairsHandrailLeftOverlap %s && bStairsHandrailRightOverlap %s && bIsOpened %s"),
	//        bStairsOverlap ? TEXT("true") : TEXT("false"),
	//        bStairsHandrailLeftOverlap ? TEXT("true") : TEXT("false"),
	//        bStairsHandrailRightOverlap ? TEXT("true") : TEXT("false"),
	//        bIsOpened ? TEXT("true") : TEXT("false")
	// )
	if (bStairsOverlap
		// && bStairsHandrailLeftOverlap && bStairsHandrailRightOverlap 
		&& bIsOpened) {
		if (Character) {
			UE_LOG(LogTemp, Error, TEXT("Go Upstairs"))
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
	else {
		if (Character) {
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

void ATwoAxisDoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult) {
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	bStairsOverlap = true;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxBeginOverlap"))
}

void ATwoAxisDoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnBoxEndOverlap(OverlappedComponent, OtherActor,
	                       OtherComponent, OtherBodyIndex);
	bStairsOverlap = false;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxEndOverlap"))
	if (Character) {
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ATwoAxisDoor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult) {
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep,
	                            SweepResult);
	bStairsOverlap = true;
}

void ATwoAxisDoor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
	bStairsOverlap = false;
}

void ATwoAxisDoor::OnBoxHandrailLeftBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                 bool bFromSweep, const FHitResult& SweepResult) {
	bStairsHandrailLeftOverlap = true;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxHandrailLeftBeginOverlap"))
}

void ATwoAxisDoor::OnBoxHandrailLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	bStairsHandrailLeftOverlap = false;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxHandrailLeftEndOverlap"))
}

void ATwoAxisDoor::OnBoxHandrailRightBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                  bool bFromSweep, const FHitResult& SweepResult) {
	bStairsHandrailRightOverlap = true;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxHandrailRightBeginOverlap"))
}

void ATwoAxisDoor::OnBoxHandrailRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	bStairsHandrailRightOverlap = false;
	UE_LOG(LogTemp, Error, TEXT("Go OnBoxHandrailRightEndOverlap"))
}
