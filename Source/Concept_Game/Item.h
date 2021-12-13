// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effects.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AMainCharacter;
UENUM(BlueprintType)
enum class EItemRarity : uint8 {
	EIR_Unspecified UMETA(DisplayName = "Unspecified"),
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Jammed UMETA(DisplayName = "Jammed"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8 {
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterp UMETA(DisplayName = "Equip Interp"),
	EIS_PickedUp UMETA(DisplayName = "Picked Up"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIS_Interact UMETA(DisplayName = "Interacting"),
	EIS_Static UMETA(DisplayName = "Static"),
	EIS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UENUM(BlueprintType)
enum class EItemType: uint8 {
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_PlotItem UMETA(DisplayName = "Plot Item"),
	EIT_Book UMETA(DisplayName = "Book"),
	EIT_Perk UMETA(DisplayName = "Perk"),
	EIT_Letter UMETA(DisplayName = "Letter"),
	EIT_Email UMETA(DisplayName = "Email"),
	EIT_PC UMETA(DisplayName = "PC"),
	EIT_Radio UMETA(DisplayName = "Radio"),
	EIT_Cache UMETA(DisplayName = "Cache"),
	EIT_Drawer UMETA(DisplayName = "Drawer"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemInventoryType: uint8 {
	EIIT_Changeable UMETA(DisplayName = "Changeable Inventory"),
	EIIT_Plot UMETA(DisplayName = "Plot Inventory"),
	EIIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT()
struct FItemDataTable : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemDimensions;
};

USTRUCT()
struct FItemRarityTable : public FTableRowBase {
	GENERATED_BODY()

	//TODO: Create different classes for effects. Do not keep it in item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Droppable;
};

UCLASS()
class CONCEPT_GAME_API AItem : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	virtual void InteractWithItem(AMainCharacter* InCharacter);

	virtual void PerformTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids);

	virtual void LeaveTrace(AMainCharacter* InMainCharacter, TArray<FGuid> Guids);

	template <typename T>
	T* RegisterNewComponent(FName ComponentName, FTransform MeshTransform);

	FTransform GetTransformFromRootComponent(const USceneComponent* InRootComponent);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void SetItemProperties(EItemState State);


	void FinishInterp();

	void ItemInterp(float DeltaTime);

	FVector GetInterpLocation();

	void PlayPickupSound(bool bForcePlaySound = false);

	//TODO: Create different classes for effects. Do not keep it in item.
	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEquipSound(bool bForcePlaySound = false);

	void ThrowItem();

	void StopFalling();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* Character;

	FTimerHandle ThrowItemTimer;

	float ThrowItemTime;

	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemInteractionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;

	FGuid ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemInventoryType ItemInventoryType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterp;

	FTimerHandle ItemInterpTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	float ItemInterpX;

	float ItemInterpY;

	float InterpInitialYawOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocationIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	int MaterialIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	FGlowMaterial GlowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndexX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndexY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint ItemDimensions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory", meta = (AllowPrivateAccess = "true"))
	bool bCharacterInventoryFull;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data Table", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemRarityDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rarity", meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rarity", meta = (AllowPrivateAccess = "true"))
	FCustomDepth CustomDepth;

public:
	FORCEINLINE FGuid GetGuid() const {
		return ID;
	}

	//TODO: Change name to InteractWidget...
	FORCEINLINE UWidgetComponent* GetPickupWidget() const {
		return PickupWidget;
	}

	FORCEINLINE USphereComponent* GetAreaSphere() const {
		return AreaSphere;
	}

	FORCEINLINE UBoxComponent* GetCollisionBox() const {
		return CollisionBox;
	}

	void SetPickupWidget(UWidgetComponent* InPickupWidget) {
		PickupWidget = InPickupWidget;
	}

	void SetAreaSphere(USphereComponent* InAreaSphere) {
		AreaSphere = InAreaSphere;
	}

	void SetCollisionBox(UBoxComponent* InCollisionBox) {
		CollisionBox = InCollisionBox;
	}

	FORCEINLINE EItemState GetItemState() const {
		return ItemState;
	}

	FORCEINLINE void SetItemDimensions(FIntPoint InItemDimensions) {
		ItemDimensions = InItemDimensions;
	}

	FIntPoint GetItemDimensions() {
		return ItemDimensions;
	}

	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const {
		return ItemMesh;
	}

	FORCEINLINE USoundCue* GetPickupSound() const {
		return PickupSound;
	}

	FORCEINLINE USoundCue* GetEquipSound() const {
		return EquipSound;
	}

	FORCEINLINE void SetPickupSound(USoundCue* InPickupSound) {
		PickupSound = InPickupSound;
	}

	FORCEINLINE void SetEquipSound(USoundCue* InEquipSound) {
		EquipSound = InEquipSound;
	}

	FORCEINLINE void SetItemState(EItemState InItemState) {
		ItemState = InItemState;
		SetItemProperties(InItemState);
	}

	FORCEINLINE int32 GetItemCount() const {
		return ItemCount;
	}

	FORCEINLINE EItemType GetItemType() const {
		return ItemType;
	}

	FORCEINLINE int32 GetSlotIndexX() const {
		return SlotIndexX;
	}

	FORCEINLINE int32 GetSlotIndexY() const {
		return SlotIndexY;
	}

	FORCEINLINE EItemRarity GetItemRarity() const {
		return ItemRarity;
	}

	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const {
		return DynamicMaterialInstance;
	}

	FORCEINLINE void SetSlotIndexX(int32 Index) {
		SlotIndexX = Index;
	}

	FORCEINLINE void SetSlotIndexY(int32 Index) {
		SlotIndexY = Index;
	}

	FORCEINLINE void SetCharacter(AMainCharacter* InCharacter) {
		Character = InCharacter;
	}

	FORCEINLINE void SetItemName(FString InName) {
		ItemName = InName;
	}

	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) {
		MaterialInstance = Instance;
	}

	FORCEINLINE UMaterialInstance* GetMaterialInstance() const {
		return MaterialInstance;
	}

	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) {
		DynamicMaterialInstance = Instance;
	}

	FORCEINLINE FLinearColor GetGlowColor() const {
		return CustomDepth.GlowColor;
	}

	FORCEINLINE int32 GetMaterialIndex() const {
		return MaterialIndex;
	}

	FORCEINLINE void SetMaterialIndex(int32 Index) {
		MaterialIndex = Index;
	}

	FORCEINLINE void SetCharacterInventoryFull(bool bFull) {
		bCharacterInventoryFull = bFull;
	}
};

template <typename T>
T* AItem::RegisterNewComponent(FName ComponentName, FTransform MeshTransform) {
	T* Component = NewObject<T>(this, ComponentName);

	Component->SetWorldTransform(MeshTransform, false, nullptr, ETeleportType::TeleportPhysics);

	Component->RegisterComponent();
	Component->OnComponentCreated();

	Component->SetRelativeTransform(MeshTransform);

	return Component;
}
