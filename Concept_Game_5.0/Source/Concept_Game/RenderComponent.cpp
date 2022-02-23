// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderComponent.h"

#include "MainCharacter.h"


// Sets default values for this component's properties
URenderComponent::URenderComponent(): bApplyFog(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InvisibilityTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("InvisibilityTransitionTimeline"));

}


// Called when the game starts
void URenderComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	PrepareInvisibility();
}


// Called every frame
void URenderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckVisibility();
	// ...
}

//TODO: Finish....
void URenderComponent::ToggleInvisibility() {
	if (bApplyInvisibility) {
		bInvisible = !bInvisible;
		if (bInvisible) {
			OwningCharacter->GetInvisibleCharacterMesh()->SetVisibility(true);
		}
		bInvisible ? InvisibilityTransitionTimeline->PlayFromStart() : InvisibilityTransitionTimeline->ReverseFromEnd();
	}
}

void URenderComponent::CheckVisibility() {
	if (OwningCharacter) {
		if (InvisibleAmount <= 0.01f && !bInvisible) {
			OwningCharacter->GetInvisibleCharacterMesh()->SetVisibility(false);
		}
	}
}

void URenderComponent::UpdateInvisibilityTransitionTimeline(float Output) {
	UE_LOG(LogTemp, Warning, TEXT("UpdateInvisibilityTransitionTimeline -> %f"), Output)
	BodyMaterial->SetScalarParameterValue(FName("Amount"), Output);
	SignMaterial->SetScalarParameterValue(FName("Amount"), Output);
	HeadMaterial->SetScalarParameterValue(FName("Amount"), Output);
	InvisibleAmount = Output;
	if (Output == 0.0f) {
		OwningCharacter->GetInvisibleCharacterMesh()->SetVisibility(false);
	}
}

void URenderComponent::PrepareInvisibility() {
	if (OwningCharacter) {
		OwningCharacter->GetInvisibleCharacterMesh()->SetVisibility(false);
		BodyMaterial = OwningCharacter->GetMesh()->CreateDynamicMaterialInstance(0);
		SignMaterial = OwningCharacter->GetMesh()->CreateDynamicMaterialInstance(1);
		HeadMaterial = OwningCharacter->GetEyesCameraHeadComponent()->CreateDynamicMaterialInstance(0);
	}

	InvisibilityFunctionFloat.BindDynamic(this, &URenderComponent::UpdateInvisibilityTransitionTimeline);
	if (InvisibilityTransitionFloatCurve) {
		InvisibilityTransitionTimeline->AddInterpFloat(InvisibilityTransitionFloatCurve, InvisibilityFunctionFloat);
		UE_LOG(LogTemp, Warning, TEXT("InvisibilityTransitionFloatCurve -> Added"))
	}
}
