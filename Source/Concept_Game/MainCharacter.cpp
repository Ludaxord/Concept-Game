// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter():
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	HipTurnRate(90.0f),
	HipLookUpRate(90.0f),
	AimingTurnRate(20.0f),
	AimingLookUpRate(20.0f),
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f),
	bAiming(false),
	bAttackButtonPressed(false),
	bAimingButtonPressed(false),
	CrouchGroundFriction(100.0f),
	CrawlingGroundFriction(100.0f),
	BaseGroundFriction(2.0f),
	bShouldAttack(false),
	CameraDefaultFOV(0.0f),
	CameraZoomedFOV(35.0f),
	CameraCurrentFOV(0.0f),
	bCrouching(false),
	bRunning(false),
	CombatState(ECombatState::ECS_Unoccupied),
	CrouchMovementSpeed(300.0f),
	RunningMovementSpeed(650.0f),
	CrawlingMovementSpeed(250.0f),
	AimingMovementSpeed(350.0f),
	BaseMovementSpeed(350.0f),
	CrouchCharacterVisibility(50.0f),
	RunningCharacterVisibility(100.0f),
	CrawlingCharacterVisibility(30.0f),
	BaseCharacterVisibility(80.0f),
	StandingCapsuleHalfHeight(88.0f),
	CrouchingCapsuleHalfHeight(44.0f),
	CrawlingCapsuleHalfHeight(22.0f),
	Health(100.0f),
	MaxHealth(100.0f) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructCameraBoom();
	ConstructFollowCamera();

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	if (FollowCamera) {
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	//TODO: Add Weapon and inventory (Depend on game progress)
	//TODO: Add Ammo (Depend on game progress)
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

	PlayerInputComponent->BindAction("Running", IE_Pressed, this, &AMainCharacter::RunningButtonPressed);
	PlayerInputComponent->BindAction("Running", IE_Released, this, &AMainCharacter::RunningButtonReleased);
	PlayerInputComponent->BindAction("PoseChange", IE_Pressed, this, &AMainCharacter::ChangePoseButtonPressed);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StopJumping);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &AMainCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &AMainCharacter::AimingButtonReleased);
	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed, this, &AMainCharacter::UseWeaponButtonPressed);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this, &AMainCharacter::UseWeaponButtonReleased);
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

void AMainCharacter::ConstructCameraBoom() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
}

void AMainCharacter::ConstructFollowCamera() {
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "head"); // Attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	FollowCamera->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
}

void AMainCharacter::UseWeapon() {
}

void AMainCharacter::AimingButtonPressed() {
	bAimingButtonPressed = true;
	if (CombatState != ECombatState::ECS_Reloading && CombatState != ECombatState::ECS_Equipping && CombatState !=
		ECombatState::ECS_Stunned)
		Aim();
}

void AMainCharacter::AimingButtonReleased() {
	bAimingButtonPressed = false;
	StopAiming();
}

void AMainCharacter::RunningButtonPressed() {
}

void AMainCharacter::RunningButtonReleased() {
}

void AMainCharacter::ChangePoseButtonPressed() {
	//TODO: Fix, Add Enum to check Current Pose...
	if (!bCrouching && !bCrawling) {
		UE_LOG(LogTemp, Error, TEXT("Crouch From Stand"));
		Crouching();
	}
	else if (bCrouching && !bCrawling) {
		UE_LOG(LogTemp, Error, TEXT("Crawl From Crouch"));
		bCrouching = false;
		Crawling();
	}
	else if (!bCrouching && bCrawling) {
		UE_LOG(LogTemp, Error, TEXT("Crouch From Crawl"));
		bCrawling = false;
		Crouching();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Stand"));
		Standing();
	}

	UE_LOG(LogTemp, Warning, TEXT("Crouching: %s, Crawling %s"), (bCrouching ? TEXT("true" ): TEXT("false")),
	       (bCrawling ? TEXT("true" ): TEXT("false")));
}

void AMainCharacter::Jump() {
	Super::Jump();
}

void AMainCharacter::Crouching() {
	if (!GetCharacterMovement()->IsFalling())
		bCrouching = true;

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrouchGroundFriction;
}

void AMainCharacter::Crawling() {
	if (!GetCharacterMovement()->IsFalling())
		bCrawling = true;

	GetCharacterMovement()->MaxWalkSpeed = CrawlingMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrawlingGroundFriction;
}

void AMainCharacter::Standing() {
	if (!GetCharacterMovement()->IsFalling()) {
		bCrawling = false;
		bCrouching = false;
	}

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	GetCharacterMovement()->GroundFriction = BaseGroundFriction;
}

void AMainCharacter::Aim() {
}

void AMainCharacter::StopAiming() {
}

void AMainCharacter::AimingFieldOfView() {
}

void AMainCharacter::UseWeaponButtonPressed() {
}

void AMainCharacter::UseWeaponButtonReleased() {
}

void AMainCharacter::StartFireTimer() {
}

void AMainCharacter::AutoFireReset() {
}

void AMainCharacter::PlayMontage(ECharacterMontage CharacterMontage) {
}

void AMainCharacter::GrapClip() {
}

void AMainCharacter::ReleaseClip() {
}
