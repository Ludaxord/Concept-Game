// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALadder::ALadder(): RungsNumber(10),
                    SpaceBetweenRungs(40.0f),
                    bTouchingLadder(false),
                    bOnSphereOverlap(false),
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

	SetItemState(EItemState::EIS_Interact);
}

void ALadder::BeginPlay() {
	Super::BeginPlay();

	LadderCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	LadderCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
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
	GetCollisionBox()->SetBoxExtent(FVector(40.0f, 50.0f, BoxLocationZ));
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

void ALadder::OnSphereBeginOverlap(UPrimitiveComponent* MovieSceneBlends, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {
	Super::OnSphereBeginOverlap(MovieSceneBlends, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bOnSphereOverlap = true;
		}
	}
}

void ALadder::OnSphereEndOverlap(UPrimitiveComponent* MovieSceneBlends, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnSphereEndOverlap(MovieSceneBlends, OtherActor, OtherComponent, OtherBodyIndex);
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bTouchingLadder = false;
			bOnSphereOverlap = false;
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Character->SetPoseType(EPoseType::EPT_Stand);
		}
	}
}

void ALadder::OnBoxBeginOverlap(UPrimitiveComponent* MovieSceneBlends, AActor* OtherActor,
                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult) {
	Super::OnBoxBeginOverlap(MovieSceneBlends, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bTouchingLadder = true;
		}
	}
}

void ALadder::OnBoxEndOverlap(UPrimitiveComponent* MovieSceneBlends, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	Super::OnBoxEndOverlap(MovieSceneBlends, OtherActor, OtherComponent, OtherBodyIndex);
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			bTouchingLadder = false;
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Character->SetPoseType(EPoseType::EPT_Stand);
		}
	}
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
	if (bTouchingLadder && bOnSphereOverlap) {
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Character->SetPoseType(EPoseType::EPT_Climb);
	}
	else if (bOnSphereOverlap) {
		//TODO: Move closer to ladder to trigger TouchingLadder bool && enable moving
		//TODO: Create Animation for movement than just transform 
		FVector Loc = CollisionBox->GetComponentLocation();
		FVector LocDiff = Character->GetActorLocation() - Loc;

		UE_LOG(LogTemp, Warning, TEXT("ActorLoc %s"), *Character->GetActorLocation().ToString())
		UE_LOG(LogTemp, Warning, TEXT("BoxLoc %s"), *Loc.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Loc Difference %s"), *LocDiff.ToString());

		float NewLocX = (Character->GetActorLocation().X > LocDiff.X)
			                ? Character->GetActorLocation().X + LocDiff.X
			                : Character->GetActorLocation().X - LocDiff.X;

		float NewLocY = (Character->GetActorLocation().Y > LocDiff.Y)
			                ? Character->GetActorLocation().Y + LocDiff.Y
			                : Character->GetActorLocation().Y - LocDiff.Y;

		float NewLocZ = (Character->GetActorLocation().Z > LocDiff.Z)
			                ? Character->GetActorLocation().Z + LocDiff.Z
			                : Character->GetActorLocation().Z - LocDiff.Z;


		Character->SetActorRelativeLocation(FVector(
			NewLocX ,
			NewLocY,
			NewLocZ
		));

		UE_LOG(LogTemp, Error, TEXT("Touching Ladder: %s and Sphere Overlap: %s"),
		       bTouchingLadder ? TEXT("true") : TEXT("false"), bOnSphereOverlap? TEXT("true") : TEXT("false"));
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Character->SetPoseType(EPoseType::EPT_Climb);
	}
	else {
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		Character->SetPoseType(EPoseType::EPT_Stand);
	}

}

void ALadder::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
	EnableClimbing();
}
