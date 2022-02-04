// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

#include "NPCInventoryComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ANPCBase::ANPCBase() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UNPCInventoryComponent>(TEXT("InventoryComponent"));

	NPCSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NPCSphere"));
	NPCSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay() {
	Super::BeginPlay();

	ID = FGuid::NewGuid();
}

// Called every frame
void ANPCBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::Interact_Implementation(AMainCharacter* InCharacter) {
	INPCInterface::Interact_Implementation(InCharacter);
}

void ANPCBase::QuestInteract_Implementation(AMainCharacter* InCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("ANPCBase::QuestInteract_Implementation"))
	Interact_Implementation(InCharacter);
}

bool ANPCBase::QuestAvailable_Implementation() {
	return IQuestHolderInterface::QuestAvailable_Implementation();
}

void ANPCBase::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult) {

}

void ANPCBase::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {

}
