// Fill out your copyright notice in the Description page of Project Settings.


#include "Cover.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACover::ACover() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CoverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoverMesh"));
	SetRootComponent(CoverMesh);

	TakeCoverCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TakeCoverCollisionBox"));
	TakeCoverCollisionBox->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACover::BeginPlay() {
	Super::BeginPlay();

	ID = FGuid::NewGuid();

	TakeCoverCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACover::OnBoxBeginOverlap);
	TakeCoverCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACover::OnBoxEndOverlap);
}

// Called every frame
void ACover::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACover::SetupCoverMeshSize() {
}

void ACover::SetupCoverSize() {
}

void ACover::SetupTakeCoverCollisionCapsule() {
}

void ACover::TakeCover() {
	if (CurrentOverlappingCharacter)
		CurrentOverlappingCharacter->EnterCover();
}

void ACover::QuitCover() {
	if (CurrentOverlappingCharacter)
		CurrentOverlappingCharacter->ExitCover();
}

void ACover::InCoverSystem() {
	if (CurrentOverlappingCharacter) {
		CurrentOverlappingCharacter->LeftTracer();
		CurrentOverlappingCharacter->RightTracer();
		CurrentOverlappingCharacter->TopTracer();
		CurrentOverlappingCharacter->TraceCharacterCover();
		CurrentOverlappingCharacter->HideCoverOnCameraTrace();

		FVector OutMoveTraceStart;
		FVector OutMoveTraceEnd;
		// GetInCoverMouseTracer(OutMoveTraceStart, OutMoveTraceEnd);

		CurrentOverlappingCharacter->PeakLeft();
		CurrentOverlappingCharacter->PeakRight();
	}

}

FVector ACover::MoveForward(float Value) {
	if (CurrentOverlappingCharacter) {
		const FRotator Rotation = CurrentOverlappingCharacter->Controller->GetControlRotation();
		const FRotator YawRotation = {Rotation.Pitch, 0, 0};
		return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}

	return FVector(0.0f);
}	

void ACover::MoveRight(float Value) {
	if (CurrentOverlappingCharacter) {
		CurrentOverlappingCharacter->MoveRightInCover(Value);
		// const FRotator Rotation = CurrentOverlappingCharacter->Controller->GetControlRotation();
		// const FRotator YawRotation = {0, Rotation.Yaw, 0};
		// if (CurrentOverlappingCharacter->GetMoveLeft() && CurrentOverlappingCharacter->GetMoveRight()) {
		// 	FHitResult Res;
		//
		// 	TArray<AActor*> IgnoredActors;
		// 	bool bTrace = UKismetSystemLibrary::SphereTraceSingle(this,
		// 	                                                      CurrentOverlappingCharacter->GetActorLocation(),
		// 	                                                      CurrentOverlappingCharacter->GetActorLocation() +
		// 	                                                      CurrentOverlappingCharacter->GetCharacterMovement()->
		// 	                                                      GetPlaneConstraintNormal()
		// 	                                                      *
		// 	                                                      -1.0f *
		// 	                                                      300.0f,
		// 	                                                      60.f,
		// 	                                                      ETraceTypeQuery::TraceTypeQuery1,
		// 	                                                      false,
		// 	                                                      IgnoredActors,
		// 	                                                      EDrawDebugTrace::Type::ForOneFrame,
		// 	                                                      Res,
		// 	                                                      true);
		// 	if (bTrace) {
		// 		CurrentOverlappingCharacter->GetCharacterMovement()->SetPlaneConstraintNormal(Res.Normal);
		// 		CurrentOverlappingCharacter->SetTraceCoverLeft(Value > 0);
		// 		CurrentOverlappingCharacter->SetTraceCoverRight(Value < 0);
		//
		// 		CurrentOverlappingCharacter->AddMovementInput(YawRotation.Quaternion().GetRightVector(), Value);
		// 	}
		// }
		// else {
		// 	float CurrentValue = 0.0f;
		// 	const float ValueSign = FMath::Sign(Value);
		// 	bool bDir;
		// 	if (ValueSign == 1.f) {
		// 		bDir = CurrentOverlappingCharacter->GetMoveRight();
		// 	}
		// 	else {
		// 		bDir = CurrentOverlappingCharacter->GetMoveLeft();
		// 	}
		//
		// 	if (ValueSign != 0.0f && bDir) {
		// 		CurrentValue = Value;
		// 	}
		//
		// 	CurrentOverlappingCharacter->SetTraceCoverLeft(CurrentValue > 0);
		// 	CurrentOverlappingCharacter->SetTraceCoverRight(CurrentValue < 0);
		//
		// 	if (!CurrentOverlappingCharacter->GetTraceCoverLeft() && CurrentValue == 0.0f) {
		// 		CurrentOverlappingCharacter->SetTraceCoverLeft(CurrentOverlappingCharacter->GetCanPeakLeft());
		// 	}
		//
		// 	if (!CurrentOverlappingCharacter->GetTraceCoverRight() && CurrentValue == 0.0f) {
		// 		CurrentOverlappingCharacter->SetTraceCoverRight(CurrentOverlappingCharacter->GetCanPeakRight());
		// 	}
		//
		// 	CurrentOverlappingCharacter->AddMovementInput(YawRotation.Quaternion().GetRightVector(), CurrentValue);
		// }
	}
}

void ACover::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("ACover Cover Capsule Overlapping BEGIN!!!!! By: %s, %s, %s"),
	       *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());
}

void ACover::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("ACover Cover Capsule Overlapping END!!!!! By: %s, %s, %s"),
	       *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());

}

void ACover::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("ACover Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
	       *OtherActor->GetName(),
	       *OverlappedComponent->GetName(), *OtherComponent->GetName());
}

void ACover::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("ACover End Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
	       *OtherActor->GetName(),
	       *OverlappedComponent->GetName(), *OtherComponent->GetName());
}

void ACover::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("ACover Box Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
	       *OtherActor->GetName(),
	       *OverlappedComponent->GetName(), *OtherComponent->GetName());
}

void ACover::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("ACover End Overlapping Actor %s OverlappedComponent: %s OtherComponent: %s"),
	       *OtherActor->GetName(),
	       *OverlappedComponent->GetName(), *OtherComponent->GetName());

}
