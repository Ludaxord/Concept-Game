// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ANPCBase::ANPCBase() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NPCCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NPCCapsule"));
	NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));

	SetRootComponent(NPCCapsule);

	NPCMesh->SetupAttachment(NPCCapsule);
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ANPCBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::Interact_Implementation() {
	INPCInterface::Interact_Implementation();
}
