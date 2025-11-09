// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleTowerDefenceGameMode.h"
#include "blueprint/UserWidget.h"
#include <Subsystems/PlayerHPSubsystem.h>
#include "Kismet/GameplayStatics.h"
#include <Level/LevelPath.h>

void ASimpleTowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UPlayerHPSubsystem* playerHpSubsystem = GetWorld()->GetSubsystem<UPlayerHPSubsystem>())
	{
		playerHpSubsystem->OnPlayerDefeated.AddDynamic(this, &ASimpleTowerDefenceGameMode::HandleGameOver);
	}

	TArray<AActor*> checked;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LevelPath"), checked);

	if (checked.Num() > 0)
	{
		LevelPathData = Cast<ALevelPath>(checked[0]);
	}

	if (LevelPathData)
	{
		LevelPathData->OnLevelCompleted.AddDynamic(this, &ASimpleTowerDefenceGameMode::HandleLevelCompleted);
	}

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->bShowMouseCursor = true;
		pc->SetInputMode(FInputModeGameAndUI());
	}
}

void ASimpleTowerDefenceGameMode::HandleGameOver()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Over! The player has been defeated. ModeHandler"));
	}
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	if (GameOverWidgetClass)
	{
		UUserWidget* gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (gameOverWidget)
		{
			gameOverWidget->AddToViewport(100);
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			if (pc)
			{
				pc->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void ASimpleTowerDefenceGameMode::HandleLevelCompleted()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Level Completed!"));
	}
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	if (LevelCompletedWidgetClass)
	{
		UUserWidget* levelCompletedWidget = CreateWidget<UUserWidget>(GetWorld(), LevelCompletedWidgetClass);
		if (levelCompletedWidget)
		{
			levelCompletedWidget->AddToViewport(100);
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			if (pc)
			{
				pc->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}