// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Level/LevelPath.h>
#include "SimpleTowerDefenceGameMode.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API ASimpleTowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Level")
	ALevelPath* LevelPathData;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LevelCompletedWidgetClass;

protected:
	UFUNCTION()
	void HandleGameOver();

	UFUNCTION()
	void HandleLevelCompleted();
};
