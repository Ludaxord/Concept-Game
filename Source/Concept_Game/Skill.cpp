// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"

void USkill::RequestAcquire() {
	RequestSkillAcquireDelegate.Broadcast(Category, this);
}

void USkill::ApplySkill(AActor* InOwner) {
	SkillStatus = ESkillStatus::ESS_Acquired;
}
