// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALadder::ALadder(): RungsNumber(10),
                    SpaceBetweenRungs(40.0f),
                    bTouchingLadder(false),
                    LadderMeshName("'/Game/_Game/Assets/Meshes/Ladder/ladder_part_pivot_center_static_mesh'") {
	ItemInteractionName = "Climb";
	ItemName = "Ladder";

	LadderMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("LadderMesh"));

	RootLadderMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
		FName("RootLadderMeshComponent"));
	SetRootComponent(RootLadderMeshComponent);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());

	LadderCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LadderCollisionCapsule"));
	LadderCollisionCapsule->SetupAttachment(GetRootComponent());
}

void ALadder::BeginPlay() {
	Super::BeginPlay();

	LadderCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	LadderCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	GetCollisionBox()->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnCollisionBoxBeginOverlap);
	GetCollisionBox()->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnCollisionBoxEndOverlap);
}

void ALadder::SetupLadderMeshSize() {
	SpaceBetweenRungs = LadderMesh->GetBounds().BoxExtent.Z * 2;
}

void ALadder::SetupLadderRungs() {
	if (LadderRungsComponents.Num() > RungsNumber) {
		this->UnregisterAllComponents();
		ReinitLadderSubComponents();
	}

	for (int i = 0; i <= RungsNumber - 1; i++) {
		FString FStaticMeshComponentName = "LadderMeshComponent_" + FString::FromInt(i);

		FTransform LadderTransform = GetTransformFromRootComponent(GetRootComponent());
		FVector FixedLadderLocation = LadderTransform.GetTranslation();
		FixedLadderLocation.Z = FixedLadderLocation.Z + (i * SpaceBetweenRungs) - LadderMesh->GetBoundingBox().Min.Z;
		LadderTransform.SetTranslation(FixedLadderLocation);
		UStaticMeshComponent* LadderMeshComponent = RegisterNewComponent<UStaticMeshComponent>(
			*FStaticMeshComponentName, LadderTransform);
		LadderMeshComponent->SetStaticMesh(LadderMesh);
		LadderMeshComponent->SetupAttachment(GetRootComponent());
		if (LadderRungsComponents.Num() < i + 1)
			LadderRungsComponents.Add(LadderMeshComponent);
	}
}

void ALadder::SetupClimbLadderBoxCollision() {
	float BoxLocationZ = (RungsNumber * SpaceBetweenRungs) / 2;
	GetCollisionBox()->SetBoxExtent(FVector(40.0f, 20.0f, BoxLocationZ));
	GetCollisionBox()->SetRelativeLocation(FVector(GetCollisionBox()->GetRelativeLocation().X,
	                                               GetCollisionBox()->GetRelativeLocation().Y,
	                                               BoxLocationZ));
	GetCollisionBox()->SetRelativeRotation(FRotator(GetCollisionBox()->GetRelativeRotation().Pitch,
	                                                GetCollisionBox()->GetRelativeRotation().Yaw,
	                                                GetCollisionBox()->GetRelativeRotation().Roll));
}

void ALadder::SetupAreaCapsule() {
	float AreaSphereLocationZ = (RungsNumber * SpaceBetweenRungs) / 2;
	LadderCollisionCapsule->SetCapsuleHalfHeight(AreaSphereLocationZ);
	LadderCollisionCapsule->SetCapsuleRadius(GetAreaSphere()->GetScaledSphereRadius());
	LadderCollisionCapsule->SetRelativeLocation(FVector(LadderCollisionCapsule->GetRelativeLocation().X,
	                                                    LadderCollisionCapsule->GetRelativeLocation().Y,
	                                                    AreaSphereLocationZ));

	LadderCollisionCapsule->SetRelativeRotation(FRotator(LadderCollisionCapsule->GetRelativeRotation().Pitch,
	                                                     LadderCollisionCapsule->GetRelativeRotation().Yaw,
	                                                     LadderCollisionCapsule->GetRelativeRotation().Roll));
}

void ALadder::ReinitLadderSubComponents() {
	RootLadderMeshComponent = RegisterNewComponent<UInstancedStaticMeshComponent>(
		FName("RootLadderMeshComponent"), GetTransformFromRootComponent(GetRootComponent())
	);
	SetRootComponent(RootLadderMeshComponent);

	// SetAreaSphere(
	// 	RegisterNewComponent<USphereComponent>(
	// 		TEXT("AreaSphere"), GetTransformFromRootComponent(GetRootComponent())
	// 	)
	// );
	// GetAreaSphere()->SetupAttachment(GetRootComponent());

	// SetCollisionBox(
	// 	RegisterNewComponent<UBoxComponent>(
	// 		TEXT("CollisionBox"), GetTransformFromRootComponent(GetRootComponent())
	// 	));
	// GetCollisionBox()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// GetCollisionBox()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// GetCollisionBox()->SetupAttachment(GetRootComponent());

	// SetPickupWidget(
	// 	RegisterNewComponent<UWidgetComponent>(
	// 		TEXT("PickupWidget"), GetTransformFromRootComponent(GetRootComponent())
	// 	));
	// GetPickupWidget()->SetupAttachment(GetRootComponent());
}

void ALadder::EnableClimbing() {
	UE_LOG(LogTemp, Error, TEXT("Touching Ladder: %s"), bTouchingLadder ? TEXT("true") : TEXT("false"));
	if (!bTouchingLadder) {
		//TODO: Move Player to enable climbing
	}
}

void ALadder::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap Collision Box By: %s"), *OtherActor->GetName());
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bTouchingLadder = true;
		}
	}

}

void ALadder::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("eND oVERLAP"));
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bTouchingLadder = false;
		}
	}
}

void ALadder::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
	EnableClimbing();
}
