// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Subsystems/ExperienceSubsystem.h>
#include "ExpSubsystemInterface.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLETOWERDEFENCE_API UExpSubsystemInterface : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

private:
	UPROPERTY()
	UExperienceSubsystem* ExpSubsystem;

	UFUNCTION()
	void HandleExperienceGained(int GainedExperiencePoints);

	UFUNCTION()
	void HandleLevelUp(int NewLevel, int RequiredExpForNextLevel);

	void UpdateUI();
};
