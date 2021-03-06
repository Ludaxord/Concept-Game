// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"

void USkill::Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount, UTexture2D* InImage) {
	Category = InCategory;
	Name = InName;
	Description = InDescription;
	PointsAmount = InPointsAmount;
	Image = InImage;
}

void USkill::RequestAcquire() {
	RequestSkillAcquireDelegate.Broadcast(Category, this);
}

void USkill::ApplySkill(AActor* InOwner) {
	SkillStatus = ESkillStatus::ESS_Acquired;
}
