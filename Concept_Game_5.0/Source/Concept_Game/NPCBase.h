// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "GameFramework/Pawn.h"
#include "NPCInterface.h"
#include "QuestHolderInterface.h"
#include "DialogHolderInterface.h"
#include "NPCInventoryComponent.h"
#include "TeamInfoInterface.h"
#include "NPCBase.generated.h"

UCLASS()
class CONCEPT_GAME_API ANPCBase : public ACharacter, public INPCInterface, public IQuestHolderInterface,
                                  public IDialogHolderInterface, public ITeamInfoInterface {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact_Implementation(AMainCharacter* InCharacter) override;

	virtual void QuestInteract_Implementation(AMainCharacter* InCharacter) override;

	virtual bool QuestAvailable_Implementation() override;

	virtual bool GoalInterrupt_Implementation(UGOAPTaskComponent* InCurrentGoal) override;

	virtual bool DialogAvailable_Implementation() override;

	virtual void DialogInteract_Implementation(AMainCharacter* InCharacter) override;

	virtual int32 GetTeamID_Implementation() override;

	virtual bool IsAlive_Implementation() override;

	virtual bool IsTargetAnEnemy_Implementation(int32 TeamID) override;

	virtual void PickupWeapon_Implementation(AWeapon* InWeapon) override;

	virtual void FireWeapon_Implementation() override;

	virtual void UseCurrentItem_Implementation() override;

	virtual void DropItem_Implementation(AItem* ItemToDrop) override;

	virtual AItem* GetEquippedItem_Implementation() override;

	void EquipWeapon(AWeapon* WeaponToEquip, FName SocketName = "RightHandSocket", bool bSwapping = false);

	void UseWeaponByType(EWeaponType WeaponType);

	void SetTestWeapon();

	AWeapon* SpawnDefaultWeapon(EWeaponType WeaponType = EWeaponType::EWT_Any);

	template <typename T>
	T* SpawnWeapon(TSubclassOf<T> WeaponClass);

	void PlayCharacterSound(ECharacterSoundState CharacterSoundState);

	void PlayMontage(ECharacterMontage CharacterMontage, EWeaponType WeaponType);

	void PerformAttack();

	virtual void SetGoals();

	virtual void SetTasks();

	virtual void AttachActorsToGOAP();

	virtual void NPCEquipWeapon(EInventoryWeapon InInventoryWeapon);

	virtual TArray<class UGOAPGoalComponent*> InitGoals_Implementation() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                                 bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC", meta = (AllowPrivateAccess = "true"))
	FString NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "NPC Mesh", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NPCSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "GOAP", meta = (AllowPrivateAccess = "true"))
	bool bQuestWidgetActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta = (AllowPrivateAccess = "true"))
	class UNPCInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta = (AllowPrivateAccess = "true"))
	class AAIController* OwningAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<class UGOAPGoalComponent*> GOAPGoalComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	TArray<class UGOAPTaskComponent*> TasksComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals", meta = (AllowPrivateAccess = "true"))
	class UGOAPFreeRoamGoalComponent* GoalFreeRoam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals", meta = (AllowPrivateAccess = "true"))
	class UGOAPGoalPatrolComponent* GoalPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals", meta = (AllowPrivateAccess = "true"))
	class UGOAPDialogGoalComponent* GoalDialog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals", meta = (AllowPrivateAccess = "true"))
	class UGOAPEscapeGoalComponent* GoalEscape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals", meta = (AllowPrivateAccess = "true"))
	class UGOAPDefeatEnemyGoalComponent* GoalDefeatEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC State", meta = (AllowPrivateAccess = "true"))
	bool bNPCDisabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	class AWorldStateManager* StateManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GOAP", meta = (AllowPrivateAccess = "true"))
	class AWorldStateManager* LocalStateManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	class UNPCDialogComponent* NPCDialogComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Team", meta =(AllowPrivateAccess = "true"))
	int32 TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFireWeapon> DefaultFireWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACyberWeapon> DefaultCyberWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AThrowableWeapon> DefaultThrowableWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeleeWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	EPoseType PoseType;

	EPoseType LastPoseType;

	FGuid ID;

	bool bGoalSet;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Task", meta = (AllowPrivateAccess = "true"))
	FString NPCCurrentTask;

	bool bUpdateGoals;

	FORCEINLINE AWeapon* GetEquippedWeapon() const {
		return EquippedWeapon;
	}

	FORCEINLINE USphereComponent* GetNPCSphere() const {
		return NPCSphere;
	}

	FORCEINLINE FString GetNPCName() const {
		return NPCName;
	}

	FORCEINLINE UNPCInventoryComponent* GetInventoryComponent() const {
		return InventoryComponent;
	}

	FORCEINLINE TArray<class UGOAPGoalComponent*> GetGOAPGoalComponents() const {
		return GOAPGoalComponents;
	}

	FORCEINLINE TArray<class UGOAPTaskComponent*> GetGOAPTasksComponents() const {
		return TasksComponents;
	}

	FORCEINLINE AAIController* GetAIController() const {
		return OwningAIController;
	}

	void SetAIController(AAIController* InOwningAIController) {
		OwningAIController = InOwningAIController;
	}

	FORCEINLINE AWorldStateManager* GetStateManager() const {
		return StateManager;
	}

	FORCEINLINE bool GetQuestWidgetActive() const {
		return bQuestWidgetActive;
	}

	FORCEINLINE void SetQuestWidgetActive(bool bInQuestWidgetActive) {
		bQuestWidgetActive = bInQuestWidgetActive;
	}
};
