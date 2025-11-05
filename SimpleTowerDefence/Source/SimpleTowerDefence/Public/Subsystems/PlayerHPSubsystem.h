// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerHPSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, CurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDefeated);

UCLASS()
class SIMPLETOWERDEFENCE_API UPlayerHPSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 StartingHealth = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 CurrentHealth = 10;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);
		CurrentHealth = StartingHealth;
	}

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(int32 Amount);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDefeated OnPlayerDefeated;
};
