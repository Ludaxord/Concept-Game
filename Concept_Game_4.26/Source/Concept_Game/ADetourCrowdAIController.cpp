// Fill out your copyright notice in the Description page of Project Settings.


#include "ADetourCrowdAIController.h"

#include "Navigation/CrowdFollowingComponent.h"

AADetourCrowdAIController::AADetourCrowdAIController(const FObjectInitializer& ObjInitializer): Super(
	ObjInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {
}
