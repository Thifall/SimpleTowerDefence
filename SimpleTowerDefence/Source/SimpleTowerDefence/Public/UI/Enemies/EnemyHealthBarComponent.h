// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIMPLETOWERDEFENCE_API UEnemyHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UEnemyHealthBarComponent();

	virtual void BeginPlay() override;

	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
};
