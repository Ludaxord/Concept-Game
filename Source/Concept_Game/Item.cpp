// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AItem::AItem(): ItemName(FString("Default")),
                ItemCount(0),
                ThrowItemTime(1.0f),
                ItemRarity(EItemRarity::EIR_Common),
                ItemState(EItemState::EIS_Pickup),
                ItemInteractionName("Pickup") {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay() {
	Super::BeginPlay();

	if (PickupWidget)
		PickupWidget->SetVisibility(false);

	ID = FGuid::NewGuid();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	SetItemProperties(ItemState);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Overlapping Begin item %s"), * GetName());
			// SphereOverlapBegin();
			OtherCharacter->SphereOverlapBegin();
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Overlapping End item %s"), * GetName());
			OtherCharacter->SphereOverlapEnd();
			// OtherCharacter->IncrementOverlappedItemCount(-1, ID);
			// OtherCharacter->UnHighlightInventorySlot();
		}
	}
}

void AItem::SetItemProperties(EItemState State) {
	switch (State) {

	case EItemState::EIS_Pickup: {
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		ItemInteractionName = "Pickup";
	}
	break;
	case EItemState::EIS_EquipInterp: {

	}
	break;
	case EItemState::EIS_PickedUp: {
		PickupWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ItemInteractionName = "";
	}
	break;
	case EItemState::EIS_Equipped: {
		PickupWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ItemInteractionName = "";
	}
	break;
	case EItemState::EIS_Falling: {
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_WorldStatic,
			ECollisionResponse::ECR_Block);
		// Set AreaSphere properties
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Set CollisionBox properties
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ItemInteractionName = "Pickup";
	}
	break;
	case EItemState::EIS_Interact: {

		ItemInteractionName = "Use";
	}
	break;
	case EItemState::EIS_Static: {

		ItemInteractionName = "Use";
	}
	break;
	}
}

void AItem::InteractWithItem(AMainCharacter* InCharacter) {
	Character = InCharacter;
}

void AItem::PerformTrace(AMainCharacter* InMainCharacter) {
	Character = InMainCharacter;
	FVector MeshPosition;
	FVector WidgetPosition;
	auto MeshWorldPosition = GetItemMesh()->GetComponentLocation();
	auto WidgetWorldPosition = PickupWidget->GetComponentLocation();

	PickupWidget->SetVisibility(true);

	FVector Difference = MeshWorldPosition - WidgetWorldPosition;
	DrawDebugLine(GetWorld(), MeshWorldPosition, WidgetWorldPosition, FColor::Red, false, 50.0f);

	UE_LOG(LogTemp, Warning, TEXT("Interaction Name: %s Item Name %s, Difference %s"),
	       *ItemInteractionName, *ItemName, *Difference.ToString())
}

void AItem::LeaveTrace(AMainCharacter* InMainCharacter) {
	Character = InMainCharacter;
	PickupWidget->SetVisibility(false);
}

void AItem::FinishInterp() {
}

void AItem::ItemInterp(float DeltaTime) {
}

FVector AItem::GetInterpLocation() {
	if (Character == nullptr) return FVector(0.0f);
	switch (ItemType) {

	case EItemType::EIT_Ammo: break;
	case EItemType::EIT_PlotItem: break;
	case EItemType::EIT_Book: break;
	case EItemType::EIT_Perk: break;
	case EItemType::EIT_Letter: break;
	case EItemType::EIT_Email: break;
	case EItemType::EIT_PC: break;
	case EItemType::EIT_Radio: break;
	case EItemType::EIT_Cache: break;
	case EItemType::EIT_Drawer: break;
	case EItemType::EIT_Weapon: break;
	case EItemType::EIT_MAX: break;
	default: ;
	}

	return FVector(0.0f);
}

void AItem::PlayPickupSound(bool bForcePlaySound) {
}

void AItem::InitializeCustomDepth() {
}

void AItem::OnConstruction(const FTransform& Transform) {

	//TODO: Set item rarity;
	if (GetMaterialInstance()) {
		SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
		GetDynamicMaterialInstance()->SetVectorParameterValue(GlowMaterial.VectorParameterName, GlowMaterial.GlowColor);
		GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());
		GlowMaterial.EnableGlowMaterial();
	}
}

void AItem::PlayEquipSound(bool bForcePlaySound) {
}

void AItem::ThrowItem() {
	FRotator MeshRotation = {0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.0f};
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	FVector MeshForward = GetItemMesh()->GetForwardVector();
	FVector MeshRight = GetItemMesh()->GetRightVector();
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-40.0f, MeshForward);

	float RandomRotation = FMath::FRandRange(0.0f, 100.0f);
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.0f, 0.0f, 1.0f));
	ImpulseDirection *= 200.0f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;

	GetWorldTimerManager().SetTimer(ThrowItemTimer, this, &AItem::StopFalling, ThrowItemTime);

	//TODO: Enable glow material...
}

void AItem::StopFalling() {
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
}

// Called every frame
void AItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//TODO: Create falling rotation function
	if (GetItemState() == EItemState::EIS_Falling && bFalling) {
		FRotator MeshRotation = {
			GetItemMesh()->GetComponentRotation().Pitch,
			GetItemMesh()->GetComponentRotation().Yaw,
			GetItemMesh()->GetComponentRotation().Roll
		};
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}
