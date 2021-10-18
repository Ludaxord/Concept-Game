// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AItem::AItem() {
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

}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult) {
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
}

void AItem::SetItemProperties(EItemState State) {
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
