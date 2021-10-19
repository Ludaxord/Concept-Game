// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AItem::AItem(): ItemName(FString("Default")),
                ItemCount(0),
                ItemRarity(EItemRarity::EIR_Common),
                ItemState(EItemState::EIS_Pickup) {
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
			// OtherCharacter->IncrementOverlappedItemCount(1, ID);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter != nullptr) {
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
	}
	break;
	case EItemState::EIS_Interact: {
	}
	break;
	case EItemState::EIS_Static: {
	}
	break;
	}
}

void AItem::InteractWithItem() {
}

void AItem::PerformInteraction() {
}

void AItem::LeaveInteraction() {
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

// Called every frame
void AItem::Tick(float DeltaTime) {

}
