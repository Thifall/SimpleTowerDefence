// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemies/EnemyBase.h"
#include "MainUICanvas.generated.h"

/**
 * 
 */
UCLASS()
class UMainUICanvas : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddEnemyHealthBar(AEnemyBase* enemy);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveEnemyHealthBar(AEnemyBase* enemy);

protected:
	//TMap<AEnemyBase, UUserWidget*> EnemyHealthBars;
};
