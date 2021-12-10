// Fill out your copyright notice in the Description page of Project Settings.


#include "PieMenu.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// UPieMenu::UPieMenu(): SectorCount(6), SectorSize(0), CurrentSectorAngle(0.0f) {
// }


void UPieMenu::SetSectorsToPieMenuWidget() {
	if (PieMenuMaterialInstance) {
		PieMenuMaterialInstance->SetScalarParameterValue("SectorCount", SectorCount);
		SectorSize = 360 / SectorCount;
		GetSectorBounds(SectorCount);

		float PieMenuSectorAngle = CurrentSectorAngle;
		if (CurrentSectorAngle == 0.0f) {
			PieMenuSectorAngle = GetCurrentSector(GetProperRotation());
		}

		PieMenuMaterialInstance->SetScalarParameterValue("ChosenAngle", PieMenuSectorAngle);
	}
}

void UPieMenu::GetSectorBounds(int InSectorCount) {
	for (int i = 1; i < InSectorCount; i++) {
		int SectorMax = (i * SectorSize) + (SectorSize / 2);
		int SectorMin = (SectorMax - SectorSize);
		SectorListMaxBounds.AddUnique(SectorMax);
		SectorListMinBounds.AddUnique(SectorMin);
	}
}

void UPieMenu::UpdatePieMenuSector() {
	if (PieMenuMaterialInstance) {
		PieMenuMaterialInstance->SetScalarParameterValue("SearchAngle", GetProperRotation());
		CurrentSectorAngle = GetCurrentSector(GetProperRotation());
		PieMenuMaterialInstance->SetScalarParameterValue("ActiveAngle", CurrentSectorAngle);
	}
}

float UPieMenu::GetCurrentSector(float InCurrentAngle) {
	int i = 0;
	for (int SectorMinBound : SectorListMinBounds) {
		if (SectorListMaxBounds.IsValidIndex(i)) {
			float SectorMinBounds = SectorListMinBounds[i];
			float SectorMaxBounds = SectorListMaxBounds[i];
			bool bInRange = UKismetMathLibrary::InRange_IntInt(UKismetMathLibrary::FTrunc(InCurrentAngle),
			                                                   SectorMinBounds,
			                                                   SectorMaxBounds);
			int CurrentSector = 0;
			if (bInRange) {
				CurrentSector = i;
				return (CurrentSector + 1) * SectorSize;
			}

			if (SectorMaxBounds > 360) {
				CurrentSector = SectorCount;
				return (CurrentSector * SectorSize);
			}

		}
		i++;
	}

	return 0.0f;
}

float UPieMenu::GetProperRotation() {
	float MousePositionX = 0.0f;
	float MousePositionY = 0.0f;
	bool isMouseConnected = GetOwningPlayer()->GetMousePosition(MousePositionX, MousePositionY);
	FVector MousePosition = {MousePositionX, MousePositionY, 0.0f};
	FVector2D ViewportSize2D = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector ViewportSize = {ViewportSize2D.X, ViewportSize2D.Y, 0.0f};
	return 180.f - UKismetMathLibrary::FindLookAtRotation(MousePosition, ViewportSize).Yaw;
}
