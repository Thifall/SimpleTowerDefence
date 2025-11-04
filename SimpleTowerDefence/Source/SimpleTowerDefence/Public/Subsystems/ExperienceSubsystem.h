// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ExperienceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceGainedSignature, int, GainedExperiencePoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUpSignature, int, NewLevel, int, RequiredExpForNextLevel);

UCLASS(BlueprintType)
class SIMPLETOWERDEFENCE_API UExperienceSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UExperienceSubsystem();
	void AddExperiencePoints(int experiencePoitns);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	int GetCurrentLevel() const { return _currentLevel; }

	UFUNCTION(BlueprintCallable, Category = "Experience")
	int GetCurrentExperience() const { return _currentExperience; }

	UFUNCTION(BlueprintCallable, Category = "Experience")
	int GetExperienceForNextLevel();

	UPROPERTY(BlueprintAssignable, Category = "Experience")
	FOnExperienceGainedSignature OnExperienceGained;

	UPROPERTY(BlueprintAssignable, Category = "Experience")
	FOnLevelUpSignature OnLevelUp;

private:
	int _currentLevel = 1;
	int _currentExperience = 0;
	TMap<int, int> _experienceTable;
};