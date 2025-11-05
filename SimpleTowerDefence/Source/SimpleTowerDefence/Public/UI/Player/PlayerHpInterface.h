// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/HorizontalBox.h>
#include <Subsystems/PlayerHPSubsystem.h>
#include <Components/Border.h>
#include "PlayerHpInterface.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API UPlayerHpInterface : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ContainerBox;

	UPROPERTY(EditAnywhere, Category = "HPInterface")
	TSubclassOf<UUserWidget> HpBarClass;

	UPROPERTY(EditAnywhere, Category = "HPInterface")
	FLinearColor ActiveBarColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, Category = "HPInterface")
	FLinearColor InactiveBarColor = FLinearColor::Gray;

private:
	UPROPERTY()
	UPlayerHPSubsystem* PlayerHpSubsystem;

	UPROPERTY() 
	TArray<UBorder*> HpBars;

	void CreateHealthBlocks(int startingHealth);
	void UpdateHealthInterface(int currentHealth);

	UFUNCTION()
	void HandleHealthChanged(int currentHealth);
};
