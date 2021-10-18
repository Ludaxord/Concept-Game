// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AMainHUD::AMainHUD() : CrossHairHalfWidth(32.0f), CrossHairHalfHeight(32.0f) {
}

void AMainHUD::BeginPlay() {
	Super::BeginPlay();

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MainCharacter)
		Character = MainCharacter;
}

void AMainHUD::DrawHUD() {
	Super::DrawHUD();
}
