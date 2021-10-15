// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	bRunning(false),
	bSwitchToFollowCamera(false),
	bEnableCameraTransition(false),
	CombatState(ECombatState::ECS_Unoccupied),
	CrouchMovementSpeed(300.0f),
	RunningMovementSpeed(650.0f),
	PoseAxisValueCounter(0),
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
	PoseType(EPoseType::EPT_Stand),
	LastPoseType(EPoseType::EPT_Stand),
	Health(100.0f),
	MaxHealth(100.0f) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructCameraBoom();
	ConstructFollowCamera();
	ConstructEyesCamera();
	ConstructRefFollowCamera();
	ConstructRefFollowCameraArrowComponent();

	ChangeCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ChangeCameraTimeline"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	ChangeCameraTimeline->SetTimelineLength(0.1f);

	if (bEnableCameraTransition)
		UpdateCameraTimelineFloat.BindDynamic(this, &AMainCharacter::OnCameraTimelineFloatUpdate);
	FinishCameraTimelineEvent.BindDynamic(this, &AMainCharacter::OnCameraTimelineFinished);

	ChangeCameraTimeline->AddInterpFloat(ChangeCameraFloatCurve, UpdateCameraTimelineFloat);
	// ChangeCameraTimeline->SetTimelinePostUpdateFunc(UpdateCameraTimelineEvent);
	ChangeCameraTimeline->SetTimelineFinishedFunc(FinishCameraTimelineEvent);
	ChangeCameraTimeline->RegisterComponent();

	if (FollowCamera) {
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
		FollowCamera->SetActive(false);
	}

	if (EyesCamera) {
		EyesCamera->SetActive(true);
	}

	if (RefFollowCamera) {
		RefFollowCamera->SetActive(false);
	}

	//TODO: Add Weapon and inventory (Depend on game progress)
	//TODO: Add Ammo (Depend on game progress)
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	InterpCapsuleHalfHeight(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookupRate", this, &AMainCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("PoseChange", this, &AMainCharacter::ChangePoseAxisButtonPressed);
	PlayerInputComponent->BindAxis("Running", this, &AMainCharacter::RunningButtonPressed);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	// PlayerInputComponent->BindAction("Running", IE_Pressed, this, &AMainCharacter::RunningButtonPressed);
	// PlayerInputComponent->BindAction("Running", IE_Released, this, &AMainCharacter::RunningButtonReleased);
	PlayerInputComponent->BindAction("DebugChangeCamera", IE_Pressed, this, &AMainCharacter::ChangeDebugCamera);
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
	CameraBoom->TargetArmLength = 180.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.0f);
}

void AMainCharacter::ConstructFollowCamera() {
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetActive(false);
}

void AMainCharacter::ConstructEyesCamera() {
	EyesCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EyesCamera"));
	EyesCamera->SetupAttachment(GetMesh(), "head"); // Attach camera to end of boom
	EyesCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	EyesCamera->SetRelativeTransform(FTransform(FQuat(-90.0f, 0.0f, 90.0f, 0.0f)));
}

void AMainCharacter::ConstructRefFollowCamera() {
	RefFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RefFollowCamera"));
	RefFollowCamera->SetupAttachment(GetCapsuleComponent());
}

void AMainCharacter::ConstructRefFollowCameraArrowComponent() {
	RefFollowCameraRotation = CreateDefaultSubobject<UArrowComponent>(TEXT("RefFollowCameraRotation"));
	RefFollowCameraRotation->SetupAttachment(GetCapsuleComponent());
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

void AMainCharacter::RunningButtonPressed(float Value) {
	bRunning = (PoseType == EPoseType::EPT_Stand && Value > 0);
	// UE_LOG(LogTemp, Warning, TEXT("Running %s"), bRunning ? TEXT("true") : TEXT("false"))
}

void AMainCharacter::RunningButtonReleased() {
	if (PoseType == EPoseType::EPT_Stand)
		bRunning = false;
}

void AMainCharacter::ChangePoseButtonPressed(FKey Key) {
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	auto KeyTime = Player->GetInputKeyTimeDown(Key);


	switch (PoseType) {
	case EPoseType::EPT_Stand:
		Crouching();
		break;
	case EPoseType::EPT_Crouch: {
		Standing();
	}
	break;
	case EPoseType::EPT_Crawl:
		Crouching();
		break;
	default:
		Standing();
		break;
	}

	const TEnumAsByte<EPoseType> PoseEnum = PoseType;
	FString EnumAsString = UEnum::GetValueAsString(PoseEnum.GetValue());

	UE_LOG(LogTemp, Warning, TEXT("Pose: %s, Pressed Key %s, Key Time %f Key Value"), *EnumAsString,
	       *(Key.GetDisplayName().ToString()), KeyTime)
}

void AMainCharacter::ChangePoseAxisButtonPressed(float Value) {
	if (PoseAxisValueCounter > 80) {
		Crawling();
	}

	if (Value == 0)
		PoseAxisValueCounter = 0;
	else
		PoseAxisValueCounter += Value;

}

void AMainCharacter::Jump() {
	Super::Jump();
}

void AMainCharacter::Crouching() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Crouch;

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrouchGroundFriction;
}

void AMainCharacter::Crawling() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Crawl;

	GetCharacterMovement()->MaxWalkSpeed = CrawlingMovementSpeed;
	GetCharacterMovement()->GroundFriction = CrawlingGroundFriction;
	LastPoseType = PoseType;
}

void AMainCharacter::Standing() {
	if (!GetCharacterMovement()->IsFalling())
		PoseType = EPoseType::EPT_Stand;

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	LastPoseType = PoseType;
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

void AMainCharacter::InterpCapsuleHalfHeight(float DeltaTime) {
	float TargetCapsuleHalfHeight = 0;
	float InterpSpeed = 20.0f;
	switch (PoseType) {

	case EPoseType::EPT_Stand:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Crouch:
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Run:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Aim:
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
		break;
	case EPoseType::EPT_Crawl:
		TargetCapsuleHalfHeight = 34.0f;
		InterpSpeed = 1.f;
		break;
	}

	const float InterpHalfHeight = FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	                                                TargetCapsuleHalfHeight,
	                                                DeltaTime,
	                                                InterpSpeed);

	const float DeltaCapsuleHalfHeight = InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector MeshOffset = {0.0f, 0.0f, -DeltaCapsuleHalfHeight};
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AMainCharacter::ChangeDebugCamera() {
	if (!GetCharacterMovement()->IsFalling()) {
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		EyesCameraTransform = SetCameraTransform(EyesCamera, "head", true, GetMesh());
		bSwitchToFollowCamera = !bSwitchToFollowCamera;
		UE_LOG(LogTemp, Error, TEXT("Camera TPP: %s"), bSwitchToFollowCamera ? TEXT("true") : TEXT("false"))

		if (!bSwitchToFollowCamera) {
			FollowCameraTransform = SetCameraTransform(FollowCamera);
			ChangeCameraTimeline->PlayFromStart();
			// UE_LOG(LogTemp, Warning, TEXT("Camera Timeline: Play From Start"));
		}
		else {
			SetActiveCameras(true);
			FollowCameraTransform = RefFollowCamera->GetComponentTransform();
			ChangeCameraTimeline->ReverseFromEnd();
			// UE_LOG(LogTemp, Warning, TEXT("Camera Timeline: Reverse From End"));
		}
	}
}

void AMainCharacter::AutoFireReset() {
}

void AMainCharacter::PlayMontage(ECharacterMontage CharacterMontage) {
}

void AMainCharacter::GrapClip() {
}

void AMainCharacter::ReleaseClip() {
}

FTransform AMainCharacter::SetCameraTransform(UCameraComponent* Camera, FName SocketName, bool AttackComponent,
                                              USkeletalMeshComponent* Parent) const {
	Camera->DetachFromComponent(
		FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			true)
	);
	FTransform CameraTransform = Camera->GetRelativeTransform();
	if (AttackComponent) {
		Camera->AttachToComponent(Parent,
		                          FAttachmentTransformRules(
			                          EAttachmentRule::KeepWorld,
			                          EAttachmentRule::KeepWorld,
			                          EAttachmentRule::KeepWorld,
			                          true
		                          ),
		                          SocketName);
	}

	return CameraTransform;
}

void AMainCharacter::SetActiveCameras(bool FollowCameraActive) const {
	FollowCamera->SetActive(FollowCameraActive);
	EyesCamera->SetActive(!FollowCameraActive);
	RefFollowCamera->SetActive(false);
}

void AMainCharacter::OnCameraTimelineFloatUpdate(float Output) {
	FTransform CameraTrack = UKismetMathLibrary::TLerp(
		FollowCameraTransform,
		EyesCameraTransform,
		Output
	);
	FVector CameraLoc = CameraTrack.GetLocation();
	FQuat CameraRot = CameraTrack.GetRotation();
	FVector CameraScale = CameraTrack.GetScale3D();
	FollowCamera->SetRelativeTransform(FTransform(
			FRotator(CameraRot.Y, CameraRot.Z, CameraRot.X),
			CameraLoc,
			CameraScale
		)
	);

	const FRotator EyesCameraRotation = EyesCamera->GetComponentRotation();
	FVector EyesCameraLocation = EyesCamera->GetRelativeLocation();
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	UE_LOG(LogTemp, Warning,
	       TEXT("Eyes Rotation Pitch: %f Eyes Location Y: %f Transition Location Z: %f Transition Rotation Z: %f"),
	       EyesCameraRotation.Pitch, EyesCameraLocation.Y, CameraLoc.Z, CameraRot.Z, Output)
	Player->SetControlRotation(FRotator(EyesCameraRotation.Pitch, CameraRot.Z, 0.0f));
}

void AMainCharacter::OnCameraTimelineFinished() {
	UE_LOG(LogTemp, Warning, TEXT("Camera Timeline Finished"))
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	if (bSwitchToFollowCamera) {
		FollowCamera->AttachToComponent(CameraBoom,
		                                FAttachmentTransformRules(
			                                EAttachmentRule::SnapToTarget,
			                                EAttachmentRule::SnapToTarget,
			                                EAttachmentRule::SnapToTarget,
			                                true
		                                ));
		if (RefFollowCameraRotation) {
			Player->SetControlRotation(RefFollowCameraRotation->GetComponentRotation());
		}
	}
	else {
		SetActiveCameras(false);
	}
	EnableInput(Player);
}
