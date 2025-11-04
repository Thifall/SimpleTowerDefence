// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ExpSubsystemInterface.h"

void UExpSubsystemInterface::NativeConstruct()
{
	Super::NativeConstruct();
	// Pobierz subsystem
	if (UWorld* World = GetWorld())
	{
		ExpSubsystem = World->GetSubsystem<UExperienceSubsystem>();
	}

	if (!ExpSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExperienceSubsystem not found!"));
		return;
	}

	// Podepnij eventy
	ExpSubsystem->OnExperienceGained.AddDynamic(this, &UExpSubsystemInterface::HandleExperienceGained);
	ExpSubsystem->OnLevelUp.AddDynamic(this, &UExpSubsystemInterface::HandleLevelUp);

	// Inicjalizacja UI
	UpdateUI();
}

void UExpSubsystemInterface::HandleExperienceGained(int GainedExperiencePoints)
{
	UpdateUI();
}

void UExpSubsystemInterface::HandleLevelUp(int NewLevel, int RequiredExpForNextLevel)
{
	UpdateUI();
}

void UExpSubsystemInterface::UpdateUI()
{
	if (!ExpSubsystem) return;

	int CurrentExp = ExpSubsystem->GetCurrentExperience();
	int RequiredExp = ExpSubsystem->GetExperienceForNextLevel();
	int CurrentLevel = ExpSubsystem->GetCurrentLevel();

	if (LevelText)
	{
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("Poziom: %d"), CurrentLevel)));
	}

	if (ExpProgressBar && RequiredExp > 0)
	{
		float Progress = static_cast<float>(CurrentExp) / static_cast<float>(RequiredExp);
		ExpProgressBar->SetPercent(Progress);
	}
}
