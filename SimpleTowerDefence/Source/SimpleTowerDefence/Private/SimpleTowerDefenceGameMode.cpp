// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleTowerDefenceGameMode.h"
#include "blueprint/UserWidget.h"
#include <Subsystems/PlayerHPSubsystem.h>

void ASimpleTowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UPlayerHPSubsystem* playerHpSubsystem = GetWorld()->GetSubsystem<UPlayerHPSubsystem>())
	{
		playerHpSubsystem->OnPlayerDefeated.AddDynamic(this, &ASimpleTowerDefenceGameMode::HandleGameOver);
	}
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void ASimpleTowerDefenceGameMode::HandleGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over! The player has been defeated."));
	// Additional game over logic can be added here, such as transitioning to a game over screen.
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
				pc->bShowMouseCursor = true;
				pc->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}