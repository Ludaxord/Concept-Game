// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderingComponent.h"

#include "MainCharacter.h"

// Sets default values for this component's properties
URenderingComponent::URenderingComponent(): bApplyFog(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InvisibilityTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("InvisibilityTransitionTimeline"));

}


// Called when the game starts
void URenderingComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	PrepareInvisibility();
}


// Called every frame
void URenderingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//TODO: Finish....
void URenderingComponent::ToggleInvisibility() {
	if (bApplyInvisibility) {
		bInvisible = !bInvisible;
		bInvisible ? InvisibilityTransitionTimeline->PlayFromStart() : InvisibilityTransitionTimeline->ReverseFromEnd();
	}
}

void URenderingComponent::UpdateInvisibilityTransitionTimeline(float Output) {
	BodyMaterial->SetScalarParameterValue(FName("Amount"), Output);
	SignMaterial->SetScalarParameterValue(FName("Amount"), Output);
	HeadMaterial->SetScalarParameterValue(FName("Amount"), Output);
}

void URenderingComponent::PrepareInvisibility() {
	if (OwningCharacter) {
		OwningCharacter->GetInvisibleCharacterMesh()->SetVisibility(false);
		BodyMaterial = OwningCharacter->GetMesh()->CreateDynamicMaterialInstance(0);
		SignMaterial = OwningCharacter->GetMesh()->CreateDynamicMaterialInstance(1);
		HeadMaterial = OwningCharacter->GetEyesCameraHeadComponent()->CreateDynamicMaterialInstance(0);
	}

	InvisibilityFunctionFloat.BindDynamic(this, &URenderingComponent::UpdateInvisibilityTransitionTimeline);
	if (InvisibilityTransitionFloatCurve) {
		InvisibilityTransitionTimeline->AddInterpFloat(InvisibilityTransitionFloatCurve, InvisibilityFunctionFloat);
	}
}
