// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkill;
USTRUCT(BlueprintType)
struct FSkills {
	GENERATED_BODY()

	FSkills();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USkill*> Skills;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONCEPT_GAME_API USkillComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FSkills> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	int SkillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* SkillTreeWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	bool bSkillTreeVisible;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetSkillTreeVisible() {
		return bSkillTreeVisible;
	}

	UFUNCTION(BlueprintCallable)
	void ToggleSkillTree();

};
