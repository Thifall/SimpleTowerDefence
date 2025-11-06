// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleTowerDefenceGameMode.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API ASimpleTowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

protected:
	UFUNCTION()
	void HandleGameOver();
};
