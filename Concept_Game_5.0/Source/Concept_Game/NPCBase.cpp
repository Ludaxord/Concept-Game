// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

#include "GOAPDefeatEnemyGoalComponent.h"
#include "GOAPDialogGoalComponent.h"
#include "GOAPEscapeGoalComponent.h"
#include "GOAPFreeRoamGoalComponent.h"
#include "GOAPGoalPatrolComponent.h"
#include "GOAPTaskComponent.h"
#include "NPCDialogComponent.h"
#include "NPCInventoryComponent.h"
#include "WorldStateManager.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPCBase::ANPCBase(): bGoalSet(false), bQuestWidgetActive(false) {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UNPCInventoryComponent>(TEXT("InventoryComponent"));

	GoalFreeRoam = CreateDefaultSubobject<UGOAPFreeRoamGoalComponent>(TEXT("GoalFreeRoam"));
	GoalPatrol = CreateDefaultSubobject<UGOAPGoalPatrolComponent>(TEXT("GoalPatrol"));

	GoalDialog = CreateDefaultSubobject<UGOAPDialogGoalComponent>(TEXT("GoalDialog"));

	GoalDefeatEnemy = CreateDefaultSubobject<UGOAPDefeatEnemyGoalComponent>(TEXT("GoalDefeatEnemy"));
	GoalEscape = CreateDefaultSubobject<UGOAPEscapeGoalComponent>(TEXT("GoalEscape"));

	NPCDialogComponent = CreateDefaultSubobject<UNPCDialogComponent>(TEXT("NPCDialogComponent"));

	NPCSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NPCSphere"));
	NPCSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay() {
	Super::BeginPlay();

	if (bNPCDisabled) {
		SetActorTickEnabled(false);
		// SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}

	ID = FGuid::NewGuid();


	if (StateManager == nullptr) {
		StateManager = Cast<AWorldStateManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AWorldStateManager::StaticClass()));
	}

	if (LocalStateManager == nullptr) {
		LocalStateManager = NewObject<AWorldStateManager>(AWorldStateManager::StaticClass());
	}

	if (StateManager) {
		StateManager->AddState(FString("CanWalk"), 1);
		StateManager->AddState(FString("CanWalk_InLoop"), 1);
		LocalStateManager->AddState(FString("CanWalk"), 1);
		LocalStateManager->AddState(FString("CanWalk_InLoop"), 1);
	}

	SetGoals();
	SetTasks();
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
	if (StateManager) {
		StateManager->AddState(FString("Is_Talking_") + NPCName, 2, true);
	}

	bQuestWidgetActive = true;
	Interact_Implementation(InCharacter);
}

bool ANPCBase::QuestAvailable_Implementation() {
	return IQuestHolderInterface::QuestAvailable_Implementation();
}

bool ANPCBase::GoalInterrupt_Implementation(UGOAPTaskComponent* InCurrentGoal) {
	return false;
}

bool ANPCBase::DialogAvailable_Implementation() {
	return true;
}

void ANPCBase::DialogInteract_Implementation(AMainCharacter* InCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("textDialogInteract_Implementation "))
	Interact_Implementation(InCharacter);
}

int32 ANPCBase::GetTeamID_Implementation() {
	return TeamID;
}

bool ANPCBase::IsAlive_Implementation() {
	return Health > 0;
}

bool ANPCBase::IsTargetAnEnemy_Implementation(int32 InTeamID) {
	return TeamID != InTeamID;
}

void ANPCBase::PickupWeapon_Implementation(AWeapon* InWeapon) {
	EquipWeapon(InWeapon);
}

void ANPCBase::FireWeapon_Implementation() {
	if (Health <= 0.0f) return;
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (PoseType == EPoseType::EPT_Climb) return;

	UseWeaponByType(EquippedWeapon->GetWeaponType());
}

void ANPCBase::UseCurrentItem_Implementation() {

}

void ANPCBase::DropItem_Implementation(AItem* ItemToDrop) {
	if (ItemToDrop) {
		ItemToDrop->GetItemMesh()->DetachFromComponent({EDetachmentRule::KeepWorld, true});
		ItemToDrop->SetItemState(EItemState::EIS_Falling);
		ItemToDrop->ThrowItem();

		ItemToDrop = nullptr;
	}
}

void ANPCBase::EquipWeapon(AWeapon* WeaponToEquip, FName SocketName, bool bSwapping) {
	if (WeaponToEquip) {
		const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(SocketName);
		if (Socket) {
			Socket->AttachActor(WeaponToEquip, GetMesh());
		}

		//TODO: create delegate
		//TODO: Create animation for picking up weapon.
		if (EquippedWeapon == nullptr) {

		}
		else if (!bSwapping) {

		}

		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void ANPCBase::UseWeaponByType(EWeaponType WeaponType) {
	const TEnumAsByte<EWeaponType> WeaponEnum = WeaponType;
	FString EnumAsString = UEnum::GetValueAsString(WeaponEnum.GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Weapon Type: %s"), *EnumAsString);

	switch (WeaponType) {
	case EWeaponType::EWT_Melee: {
	}
	break;
	case EWeaponType::EWT_Fire: {
		PlayCharacterSound(ECharacterSoundState::ECSS_UseWeapon);
		PerformAttack();
		PlayMontage(ECharacterMontage::ECM_UseWeapon, EquippedWeapon->GetWeaponType());

		if (StateManager) {
			StateManager->AddState(FString("Fire Weapon"), 10, true);
			StateManager->SetInterruptCurrentState(true);
		}

		EquippedWeapon->DecreaseUsability();

		EquippedWeapon->StartWeaponAnimationTimer();
	}
	break;
	case EWeaponType::EWT_Force: {
	}
	break;
	case EWeaponType::EWT_Any: break;
	case EWeaponType::EWT_Throwable: break;
	case EWeaponType::EWT_MAX: break;
	default: ;
	}
}

void ANPCBase::PlayCharacterSound(ECharacterSoundState CharacterSoundState) {
}

void ANPCBase::PlayMontage(ECharacterMontage CharacterMontage, EWeaponType WeaponType) {
}

void ANPCBase::PerformAttack() {
}

void ANPCBase::SetGoals() {
	if (!bGoalSet) {
		TArray<UActorComponent*> Elements;
		GetComponents(UGOAPGoalComponent::StaticClass(), Elements);
		UE_LOG(LogTemp, Display, TEXT("GOAP NPC => %s Goals Elements %i"), *GetName(), Elements.Num())

		for (UActorComponent* Element : Elements) {
			if (UGOAPGoalComponent* Goal = Cast<UGOAPGoalComponent>(Element)) {
				UE_LOG(LogTemp, Warning, TEXT("GOAP NPC => %s SetGoal : %s"), *GetName(), *Element->GetName())
				GOAPGoalComponents.Add(Goal);
				// if (Goal->bSetAsCurrentGoal) {
				// 	UE_LOG(LogTemp, Warning, TEXT("GOAP: SetGoal : %s"), *Element->GetName())
				// 	GOAPGoalComponents.Add(Goal);
				// }
			}
		}

		bGoalSet = true;
	}
}

void ANPCBase::SetTasks() {
	TArray<UActorComponent*> Elements;
	GetComponents(UGOAPTaskComponent::StaticClass(), Elements);
	for (UActorComponent* Element : Elements) {
		if (UGOAPTaskComponent* Task = Cast<UGOAPTaskComponent>(Element)) {
			TasksComponents.Add(Task);
		}
	}
}

void ANPCBase::AttachActorsToGOAP() {
	TArray<UGOAPTaskComponent*> NPCTasks;
	GetComponents(NPCTasks);

	for (UGOAPTaskComponent* Task : NPCTasks) {
		UE_LOG(LogTemp, Display, TEXT("AttachActorsToGOAP => %s"), *Task->GetName())
		Task->CallActors();
	}

}

TArray<UGOAPGoalComponent*> ANPCBase::InitGoals_Implementation() {
	return GOAPGoalComponents;
}

void ANPCBase::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult) {

}

void ANPCBase::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {

}
