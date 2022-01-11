// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AItem::AItem(): ItemName(FString("Default")),
                ItemCount(0),
                bInteractionEnabled(true),
                ThrowItemTime(1.0f),
                bItemCurrentlyOverlapped(false),
                ItemRarity(EItemRarity::EIR_Common),
                ItemState(EItemState::EIS_Pickup),
                ItemInteractionName("Pickup") {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	// CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	// CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

FTransform AItem::GetTransformFromRootComponent(const USceneComponent* InRootComponent) {
	FVector Location = {
		InRootComponent->GetComponentLocation().X,
		InRootComponent->GetComponentLocation().Y,
		InRootComponent->GetComponentLocation().Z
	};
	FRotator Rotation = {
		InRootComponent->GetComponentRotation().Pitch,
		InRootComponent->GetComponentRotation().Yaw,
		InRootComponent->GetComponentRotation().Roll
	};

	return FTransform(Rotation, Location);
}

// Called when the game starts or when spawned
void AItem::BeginPlay() {
	Super::BeginPlay();

	if (PickupWidget)
		PickupWidget->SetVisibility(false);

	ID = FGuid::NewGuid();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	CurrentSphereRadius = AreaSphere->GetScaledSphereRadius();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AItem::OnBoxEndOverlap);

	SetItemProperties(ItemState);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Error, TEXT("Overlapping %s"), * OverlappedComponent->GetName());
	if (bInteractionEnabled) {
		if (OtherActor) {
			AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
			if (OtherCharacter != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Overlapping Begin item %s Overlapped Component %s"), *GetName(),
				       *OverlappedComponent->GetName());
				// SphereOverlapBegin();
				OtherCharacter->SphereOverlapBegin(ID);
				bItemCurrentlyOverlapped = true;
			}
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (bInteractionEnabled) {
		if (OtherActor) {
			AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
			if (OtherCharacter != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Overlapping End item %s Overlapped Component %s"), * GetName(),
				       *OverlappedComponent->GetName());
				OtherCharacter->SphereOverlapEnd(ID);
				bItemCurrentlyOverlapped = false;
				// OtherCharacter->IncrementOverlappedItemCount(-1, ID);
				// OtherCharacter->UnHighlightInventorySlot();
			}
		}
	}
}

void AItem::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Box Overlapping BEGIN!!!!!"));
}

void AItem::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Box Overlapping END!!!!!"));

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
	case EItemState::EIS_PickupDisabled: {
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

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
	if (!bInteractionEnabled && ItemState != EItemState::EIS_PickupDisabled) return;
	Character = InCharacter;
	Character->SetCurrentInteractItem(this);
	//TODO: If item is not weapon, then check if it is a usable item or story item.
	//TODO: If it is story Item, then store in story items backpack
	//TODO: If it is usable item, check if there is a space in inventory
	//TODO: If there is no place, show error message.

}

void AItem::PerformTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids) {
	Character = InMainCharacter;
	FVector MeshPosition;
	FVector WidgetPosition;
	auto MeshWorldPosition = GetItemMesh()->GetComponentLocation();
	auto WidgetWorldPosition = PickupWidget->GetComponentLocation();
	// UE_LOG(LogTemp, Warning, TEXT("Perform Traced Item => %s Contains Item => %s"), *ID.ToString(), Guids.Contains(ID) ? TEXT("Contains") : TEXT("Not Contains"))
	if (Guids.Contains(ID)) {
		PickupWidget->SetVisibility(true);
	}

	FVector Difference = MeshWorldPosition - WidgetWorldPosition;
	// DrawDebugLine(GetWorld(), MeshWorldPosition, WidgetWorldPosition, FColor::Red, false, 50.0f);

	// UE_LOG(LogTemp, Warning, TEXT("Interaction Name: %s Item Name %s, Difference %s"),
	//        *ItemInteractionName, *ItemName, *Difference.ToString())
}

void AItem::LeaveTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids) {
	Character = InMainCharacter;
	// UE_LOG(LogTemp, Warning, TEXT("Perform Traced Item => %s Contains Item => %s"), *ID.ToString(), Guids.Contains(ID) ? TEXT("Contains") : TEXT("Not Contains"))
	PickupWidget->SetVisibility(false);
}

void AItem::InteractionEnabled(bool bEnabled) {
	if (bInteractionEnabled) {
		EnableItemState = ItemState;
	}

	bInteractionEnabled = bEnabled;
	SetItemState(bInteractionEnabled ? EnableItemState : EItemState::EIS_PickupDisabled);

	// GetAreaSphere()->SetSphereRadius(bInteractionEnabled ? CurrentSphereRadius : 0.0f);
	// GetAreaSphere()->SetCollisionEnabled(bEnabled ? CurrentCollisionType : ECollisionEnabled::NoCollision);
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
	FRotator MeshRotation = {
		GetItemMesh()->GetComponentRotation().Pitch,
		GetItemMesh()->GetComponentRotation().Yaw,
		GetItemMesh()->GetComponentRotation().Roll
	};
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

void AItem::DropItemFromInventory(AActor* InActor, bool bGroundClamp) {
	float RandomRotation = FMath::FRandRange(-200.f, 200.0f);
	FVector SpawnLocation = InActor->GetActorLocation() + InActor->GetActorForwardVector() * RandomRotation;

	bool bUseGroundClamp = true;

	if (Cast<AMainCharacter>(InActor)) {
		UE_LOG(LogTemp, Warning, TEXT("Casting Actor to Main Character"))
		const USkeletalMeshSocket* HandSocket = Cast<AMainCharacter>(InActor)->GetMesh()->GetSocketByName(
			FName("RightHandSocket"));
		FVector NewSpawnLocation = HandSocket->GetSocketLocation(Cast<AMainCharacter>(InActor)->GetMesh());
		UE_LOG(LogTemp, Warning, TEXT("Change Spawn Location From: %s To: %s"), *SpawnLocation.ToString(),
		       *NewSpawnLocation.ToString())

		SpawnLocation = NewSpawnLocation;
		bUseGroundClamp = false;

		if (this == Cast<AMainCharacter>(InActor)->GetEquippedWeapon()) {
			GetItemMesh()->DetachFromComponent({EDetachmentRule::KeepWorld, true});
			Cast<AMainCharacter>(InActor)->SetEquippedWeapon(nullptr);
		}
	}

	if (bUseGroundClamp) {
		if (bGroundClamp) {
			TArray<AActor*> IgnoredActors;
			FHitResult HitResult;
			ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
			bool bHit = UKismetSystemLibrary::LineTraceSingle(this,
			                                                  SpawnLocation,
			                                                  SpawnLocation - FVector(0.0f, 0.0f, 1000.0f),
			                                                  TraceType,
			                                                  false,
			                                                  IgnoredActors,
			                                                  EDrawDebugTrace::ForOneFrame,
			                                                  HitResult,
			                                                  true,
			                                                  FLinearColor::MakeRandomColor(),
			                                                  FLinearColor::MakeRandomColor());
			if (bHit || HitResult.bBlockingHit) {
				SpawnLocation = HitResult.Location;
			}
		}
	}


	FRotator MeshRotation = {
		GetItemMesh()->GetComponentRotation().Pitch,
		GetItemMesh()->GetComponentRotation().Yaw,
		GetItemMesh()->GetComponentRotation().Roll
	};
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	// {
	// 	FVector MeshForward = GetItemMesh()->GetForwardVector();
	// 	FVector MeshRight = GetItemMesh()->GetRightVector();
	// 	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-40.0f, MeshForward);
	//
	// 	float RandomRot = FMath::FRandRange(0.0f, 100.0f);
	// 	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRot, FVector(0.0f, 0.0f, 1.0f));
	// 	ImpulseDirection *= 200.0f;
	// 	GetItemMesh()->AddImpulse(ImpulseDirection);
	// }

	bFalling = true;
	SetItemState(EItemState::EIS_Falling);

	SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);

	GetWorldTimerManager().SetTimer(ThrowItemTimer, this, &AItem::StopFalling, ThrowItemTime);

	UE_LOG(LogTemp, Warning, TEXT("Item Dropped %s"), *GetName())
}

void AItem::RotateInventoryItem() {
	bRotated = !bRotated;
}

UMaterialInstance* AItem::GetInventoryImageIcon() {
	if (bRotated) {
		return InventoryItemIcon;
	}

	return InventoryItemIconRotated;

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
