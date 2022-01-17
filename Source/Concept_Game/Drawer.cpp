// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"

#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ADrawer::ADrawer(): bMoveDrawer(false), CurrentDrawerIndex(INDEX_NONE) {
	ItemInteractionName = "Open";
	ItemName = "Drawer";

	TopDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopDrawerMesh"));
	BottomDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomDrawerMesh"));
	MiddleDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiddleDrawerMesh"));


	TopDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	BottomDrawerMesh->SetupAttachment(PhysicsBasedMesh);
	MiddleDrawerMesh->SetupAttachment(PhysicsBasedMesh);

	PhysicsBasedMesh->SetSimulatePhysics(false);
	PhysicsBasedMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	TopDrawerMesh->SetSimulatePhysics(true);
	TopDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	BottomDrawerMesh->SetSimulatePhysics(true);
	BottomDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	MiddleDrawerMesh->SetSimulatePhysics(true);
	MiddleDrawerMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	ArrowForwardComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowForwardComponent"));
	ArrowForwardComponent->SetupAttachment(PhysicsBasedMesh);

	DrawerMovementTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(
		TEXT("DrawerMovementTransitionTimeline"));

}

void ADrawer::BeginPlay() {
	Super::BeginPlay();

	Character = Cast<AMainCharacter>(GetOwner());

	DrawerMeshes.Add(BottomDrawerMesh);
	DrawerMeshes.Add(MiddleDrawerMesh);
	DrawerMeshes.Add(TopDrawerMesh);

	for (UStaticMeshComponent* DrawerMesh : DrawerMeshes) {
		FDrawerElement DrawerElement = FDrawerElement();

		DrawerElement.bIsOpened = false;
		DrawerElement.DrawerMesh = DrawerMesh;
		DrawerElement.DrawerLoc = DrawerMesh->GetComponentLocation();


		//TODO: Place AItem* in Drawer....


		Drawers.Add(DrawerElement);
	}

	DrawerMovementFunctionFloat.BindDynamic(this, &ADrawer::UpdateDrawerMovementTransitionTimeline);
	if (DrawerMovementTransitionFloatCurve) {
		DrawerMovementTransitionTimeline->AddInterpFloat(DrawerMovementTransitionFloatCurve,
		                                                 DrawerMovementFunctionFloat);
	}
}

void ADrawer::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	FindTracingComponent();
	UpdateDrawerMovement();
}


void ADrawer::FindTracingComponent() {
	FHitResult HitResult;
	FVector Loc;
	if (bItemCurrentlyOverlapped) {
		if (Character) {
			if (Character->TraceUnderCrosshairs(HitResult, Loc)) {
				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (UStaticMeshComponent* StaticMeshHitComponent = Cast<UStaticMeshComponent>(HitComponent)) {
					for (UStaticMeshComponent* DrawerMesh : DrawerMeshes) {
						if (DrawerMesh == StaticMeshHitComponent) {
							CurrentTracingDrawerMesh = StaticMeshHitComponent;
						}
					}
				}
				else {
					CurrentTracingDrawerMesh = nullptr;
				}
			}
			else {
				CurrentTracingDrawerMesh = nullptr;
			}
		}
		else {
			CurrentTracingDrawerMesh = nullptr;
		}
	}
	else {
		CurrentTracingDrawerMesh = nullptr;
	}
}

void ADrawer::UpdateDrawerMovement() {
	if (bMoveDrawer) {
		if (CurrentDrawerIndex != INDEX_NONE) {
			if (CurrentTracingDrawerMesh != nullptr) {
				FVector Loc = CurrentTracingDrawerMesh->GetComponentLocation();

				//TODO: Change Values of Forward, RIght etc vectors based on Rotation.
				ArrowForwardComponent->GetComponentRotation();

				FVector Forward = ArrowForwardComponent->GetForwardVector() * Drawers[
					CurrentDrawerIndex].DrawerLoc;

				float ForwardFloat = 0;
				if (Forward.X != 0) {
					ForwardFloat = Forward.X;
				}
				else if (Forward.Y != 0) {
					ForwardFloat = Forward.Y;
				}
				else if (Forward.Z != 0) {
					ForwardFloat = Forward.Z;
				}

				float OpenForwardFloat = ForwardFloat >= 0 ? ForwardFloat + 20 : ForwardFloat - 20;

				float NewLoc = UKismetMathLibrary::Lerp(Drawers[CurrentDrawerIndex].bIsOpened
					                                        ? OpenForwardFloat
					                                        : ForwardFloat,
				                                        Drawers[CurrentDrawerIndex].bIsOpened
					                                        ? OpenForwardFloat
					                                        : ForwardFloat,
				                                        CurrentDrawerMovement);

				FVector NLoc = {
					Forward.X != 0 ? NewLoc : Loc.X,
					Forward.Y != 0 ? NewLoc : Loc.Y,
					Forward.Z != 0 ? NewLoc : Loc.Z
				};


				UE_LOG(LogTemp, Warning, TEXT("Loc: %s Drawer: %s NLoc: %s"), *Loc.ToString(),
				       *Forward.ToString(), *NLoc.ToString())

				CurrentTracingDrawerMesh->SetWorldLocationAndRotation(NLoc,
				                                                      CurrentTracingDrawerMesh->GetComponentRotation());
			}

		}

		if (CurrentDrawerMovement == 1.f) {
			bMoveDrawer = false;
			CurrentDrawerMovement = 0;
			CurrentDrawerIndex = INDEX_NONE;
		}
	}
}

void ADrawer::InteractWithItem(AMainCharacter* InCharacter) {
	// Super::InteractWithItem(InCharacter);

	if (Character && CurrentTracingDrawerMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Interact Component: %s"),
		       *CurrentTracingDrawerMesh->GetName())
		CurrentDrawerIndex = Drawers.IndexOfByPredicate(
			[&](FDrawerElement InDrawerElement) -> bool {
				return CurrentTracingDrawerMesh == InDrawerElement.DrawerMesh;
			});

		if (CurrentDrawerIndex != INDEX_NONE) {
			Drawers[CurrentDrawerIndex].bIsOpened = !Drawers[CurrentDrawerIndex].bIsOpened;
			bMoveDrawer = true;
			DrawerMovementTransitionTimeline->PlayFromStart();
		}
	}
}

void ADrawer::UpdateDrawerMovementTransitionTimeline(float Output) {
	CurrentDrawerMovement = Output;
}
