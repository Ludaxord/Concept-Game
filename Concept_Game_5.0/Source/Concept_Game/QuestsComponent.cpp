// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestsComponent.h"

UQuestsComponent::UQuestsComponent() {
}

void UQuestsComponent::BeginPlay() {
	Super::BeginPlay();
}

void UQuestsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
