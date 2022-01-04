// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.generated.h"

/**
 * 
 */

class USkill;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRequestSkillAquireRequest, FString, InCategory, USkill*, InSkill);

UENUM(BlueprintType)
enum class ESkillStatus : uint8 {
	ESS_Locked UMETA(DisplayName= "Locked"),
	ESS_Unlocked UMETA(DisplayName= "Unlocked"),
	ESS_Acquired UMETA(DisplayName= "Acquired"),
	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS(Abstract, Blueprintable)
class CONCEPT_GAME_API USkill : public UObject {
	GENERATED_BODY()

protected:
	virtual void Init(FString InCategory, FString InName, FString InDescription, int InPointsAmount, UTexture2D* InImage);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	FString Category;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	FString Description;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	int PointsAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	UTexture2D* Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Properties", meta = (AllowPrivateAccess = "true"))
	ESkillStatus SkillStatus;
public:
	UPROPERTY(BlueprintAssignable, Category= "Delegates", meta = (AllowPrivateAccess = "true"))
	FRequestSkillAquireRequest RequestSkillAcquireDelegate;

	UFUNCTION(BlueprintCallable)
	virtual void RequestAcquire();

	UFUNCTION(BlueprintCallable)
	virtual void ApplySkill(AActor* InOwner);

	FORCEINLINE FString GetCategory() {
		return Category;
	}

	FORCEINLINE FString GetName() {
		return Name;
	}

	FORCEINLINE FString GetDescription() {
		return Description;
	}

	FORCEINLINE int GetPointsAmount() {
		return PointsAmount;
	}

	FORCEINLINE UTexture2D* GetImage() {
		return Image;
	}

	FORCEINLINE ESkillStatus GetSkillStatus() {
		return SkillStatus;
	}

	FORCEINLINE void SetCategory(FString InCategory) {
		Category = InCategory;
	}

	FORCEINLINE void SetName(FString InName) {
		Name = InName;
	}

	FORCEINLINE void SetDescription(FString InDescription) {
		Description = InDescription;
	}

	FORCEINLINE void SetPointsAmount(int InPointsAmount) {
		PointsAmount = InPointsAmount;
	}

	FORCEINLINE void SetImage(UTexture2D* InImage) {
		Image = InImage;
	}

	FORCEINLINE void SetSkillStatus(ESkillStatus InSkillStatus) {
		SkillStatus = InSkillStatus;
	}
};
