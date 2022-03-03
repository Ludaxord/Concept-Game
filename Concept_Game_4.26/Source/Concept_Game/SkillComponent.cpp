// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"

#include "MainCharacter.h"
#include "Skill.h"

FSkills::FSkills() {
}

// Sets default values for this component's properties
USkillComponent::USkillComponent(): SkillPoints(0), bSkillTreeVisible(false) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillComponent::BeginPlay() {
	Super::BeginPlay();
	OwningCharacter = Cast<AMainCharacter>(GetOwner());
	// ...

}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                    FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillComponent::AddPoint() {
	SkillPoints++;
}

void USkillComponent::ToggleSkillTree() {
	bSkillTreeVisible = !bSkillTreeVisible;
}

void USkillComponent::AddSkills(FString InCategory, TArray<USkill*> InSkills) {
	if (InSkills.Num() > 0 && InSkills.IsValidIndex(0)) {
		InSkills[0]->SetSkillStatus(ESkillStatus::ESS_Unlocked);
		FSkills Skills;
		Skills.Skills = InSkills;
		Root.Add(InCategory, Skills);
		for (USkill* Skill : InSkills) {
			Skill->RequestSkillAcquireDelegate.AddDynamic(this, &USkillComponent::OnSkillAcquired);
		}
	}
}

void USkillComponent::OnSkillAcquired(FString InCategory, USkill* InSkill) {
	if (SkillPoints > 0) {
		if (SkillPoints >= InSkill->GetPointsAmount()) {
			SkillPoints = SkillPoints - InSkill->GetPointsAmount();
			if (FSkills* FoundSkills = Root.Find(InCategory)) {
				int SkillIndex = FoundSkills->Skills.Find(InSkill);
				if (FoundSkills->Skills.IsValidIndex(SkillIndex)) {
					FoundSkills->Skills[SkillIndex]->SetSkillStatus(ESkillStatus::ESS_Acquired);
					SkillIndex++;
					if (FoundSkills->Skills.IsValidIndex(SkillIndex)) {
						FoundSkills->Skills[SkillIndex]->SetSkillStatus(ESkillStatus::ESS_Unlocked);
					}
				}
			}
			InSkill->ApplySkill(OwningCharacter);
		}

	}
}

TArray<FString> USkillComponent::GetCategories() {
	TArray<FString> Categories;
	Root.GetKeys(Categories);
	return Categories;
}

bool USkillComponent::GetSkillsArray(FString InCategory, FSkills& OutSkills) {
	if (Root.Contains(InCategory)) {
		OutSkills = Root.FindRef(InCategory);
		return true;
	}

	return false;
}
