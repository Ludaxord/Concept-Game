// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter():
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	bAiming(false),
	HipTurnRate(90.0f),
	HipLookUpRate(90.0f),
	AimingTurnRate(20.0f),
	AimingLookUpRate(20.0f),
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "head"); // Attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	FollowCamera->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookupRate", this, &AMainCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
}

void AMainCharacter::MoveForward(float Value) {
	if (Controller != nullptr && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = {0, Rotation.Yaw, 0};
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value) {
	if (Controller != nullptr && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = {0, Rotation.Yaw, 0};
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::TurnRate(float Rate) {
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Turn(float Value) {
	float TurnScaleFactor = bAiming ? MouseAimingTurnRate : MouseHipTurnRate;
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AMainCharacter::LookUpAtRate(float Rate) {
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUp(float Value) {
	float LookUpScaleFactor = bAiming ? MouseAimingLookUpRate : MouseHipLookUpRate;
	AddControllerPitchInput(Value * LookUpScaleFactor);
}
