// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALadder::ALadder(): RungsNumber(10),
                    SpaceBetweenRungs(40.0f),
                    bTouchingLadder(false),
                    bOnSphereOverlap(false),
                    LadderMeshName("'/Game/_Game/Assets/Meshes/Ladder/ladder_part_pivot_center_static_mesh'") {
	ItemInteractionName = "Climb";
	ItemName = "Ladder";

	LadderTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LadderTransitionTimeline"));

	LadderMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("LadderMesh"));
	InteractSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("InteractSceneComponent"));

	RootLadderMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
		FName("RootLadderMeshComponent"));
	SetRootComponent(RootLadderMeshComponent);

	InteractSceneComponent->SetupAttachment(GetRootComponent());
	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());

	LadderCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LadderCollisionCapsule"));
	LadderCollisionCapsule->SetupAttachment(GetRootComponent());

	LadderUpperCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LadderUpperCollisionSphere"));
	LadderUpperCollisionSphere->SetupAttachment(GetRootComponent());

	LadderBottomCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LadderBottomCollisionSphere"));
	LadderBottomCollisionSphere->SetupAttachment(GetRootComponent());

	SetItemState(EItemState::EIS_Interact);
}

void ALadder::BeginPlay() {
	Super::BeginPlay();

	UpdateFunctionFloat.BindDynamic(this, &ALadder::UpdateLadderTransitionTimeline);
	if (LadderTransitionFloatCurve) {
		LadderTransitionTimeline->AddInterpFloat(LadderTransitionFloatCurve, UpdateFunctionFloat);
	}

	LadderCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	LadderCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	LadderUpperCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnSphereOverlapFromTopBegin);
	LadderUpperCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnSphereOverlapFromTopEnd);

	LadderBottomCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnSphereOverlapFromBottomBegin);
	LadderBottomCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnSphereOverlapFromBottomEnd);
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

void ALadder::SetupBottomCapsule() {
	float AreaSphereLocationZ = GetActorLocation().GetMin();
	LadderBottomCollisionSphere->SetRelativeLocation(FVector(GetAreaSphere()->GetRelativeLocation().X,
	                                                         GetAreaSphere()->GetRelativeLocation().Y,
	                                                         GetAreaSphere()->GetRelativeLocation().Z));
}

void ALadder::SetupUpperCapsule() {
	float AreaSphereLocationZ = (RungsNumber * SpaceBetweenRungs);
	LadderUpperCollisionSphere->SetRelativeLocation(FVector(LadderCollisionCapsule->GetRelativeLocation().X,
	                                                        LadderCollisionCapsule->GetRelativeLocation().Y,
	                                                        AreaSphereLocationZ));
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
			OtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			if (OtherCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb)
				Character->SwitchCamera(false);
			OtherCharacter->SetPoseType(EPoseType::EPT_Stand);
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
			OtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			if (OtherCharacter->GetCurrentPoseType() == EPoseType::EPT_Climb)
				Character->SwitchCamera(false);
			OtherCharacter->SetPoseType(EPoseType::EPT_Stand);
			OtherCharacter->SetCurrentInteractItem(nullptr);
		}
	}
}

void ALadder::OnSphereOverlapFromTopBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlapFromTopBegin"));
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			OtherCharacter->SetOverlappingLadderTop(true);
			UE_LOG(LogTemp, Warning, TEXT("Begin GetOverlappingLadderTop %s"),
			       OtherCharacter->GetOverlappingLadderTop() ? TEXT("true") : TEXT("false"));
		}
	}
}

void ALadder::OnSphereOverlapFromTopEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlapFromTopEnd"));
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			OtherCharacter->SetOverlappingLadderTop(false);
			UE_LOG(LogTemp, Warning, TEXT("End GetOverlappingLadderTop %s"),
			       OtherCharacter->GetOverlappingLadderTop() ? TEXT("true") : TEXT("false"));
		}
	}
}

void ALadder::OnSphereOverlapFromBottomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlapFromBottomBegin"));
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			OtherCharacter->SetOverlappingLadderBottom(true);
			UE_LOG(LogTemp, Warning, TEXT("Begin GetOverlappingLadderBottom %s"),
			       OtherCharacter->GetOverlappingLadderBottom() ? TEXT("true") : TEXT("false"));
		}
	}
}

void ALadder::OnSphereOverlapFromBottomEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlapFromBottomEnd"));
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			OtherCharacter->SetOverlappingLadderBottom(false);
			UE_LOG(LogTemp, Warning, TEXT("End GetOverlappingLadderBottom %s"),
			       OtherCharacter->GetOverlappingLadderBottom() ? TEXT("true") : TEXT("false"));
		}
	}
}

void ALadder::ReinitLadderSubComponents() {
	RootLadderMeshComponent = RegisterNewComponent<UInstancedStaticMeshComponent>(
		FName("RootLadderMeshComponent"), GetTransformFromRootComponent(GetRootComponent())
	);
	SetRootComponent(RootLadderMeshComponent);
}

void ALadder::EnableClimbing() {
	if (bOnSphereOverlap) {
		//TODO: Move closer to ladder to trigger TouchingLadder bool && enable moving
		//TODO: Create Animation for movement than just transform
		ActorLocationBeforeTransition = Character->GetActorLocation();
		ActorRotationBeforeTransition = Character->GetActorRotation();
		LadderTransitionTimeline->PlayFromStart();
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Character->SetPoseType(EPoseType::EPT_Climb);
		Character->SwitchCamera(true);
		//TODO: Hide weapon if it is equipped
	}
	else {
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		if (Character->GetCurrentPoseType() == EPoseType::EPT_Climb)
			Character->SwitchCamera(false);
		Character->SetPoseType(EPoseType::EPT_Stand);
		//TODO: Show weapon if it was equipped
	}
	UE_LOG(LogTemp, Warning, TEXT("bOnSphereOverlap %s"), bOnSphereOverlap ? TEXT("true") : TEXT("false"));

}

void ALadder::UpdateLadderTransitionTimeline(float Output) {

	UE_LOG(LogTemp, Warning, TEXT("Updating Ladder Transition %s"), *FString::FromInt(Output));
	FRotator LadderForwardRot = FMath::Lerp(ActorRotationBeforeTransition,
	                                        UKismetMathLibrary::MakeRotFromX(
		                                        -GetActorRightVector()
	                                        ),
	                                        Output);
	FVector LadderForwardLoc = FMath::Lerp(ActorLocationBeforeTransition,
	                                       FVector(
		                                       InteractSceneComponent->GetComponentLocation().X,
		                                       InteractSceneComponent->GetComponentLocation().Y,
		                                       Character->GetActorLocation().Z
	                                       ),
	                                       Output);

	Character->SetActorLocationAndRotation(LadderForwardLoc, LadderForwardRot, true);
}

void ALadder::InteractWithItem(AMainCharacter* InCharacter) {
	Super::InteractWithItem(InCharacter);
	EnableClimbing();
}
