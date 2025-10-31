// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemies/EnemyHealthBarComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

UEnemyHealthBarComponent::UEnemyHealthBarComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawAtDesiredSize(true);
}

void UEnemyHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// Upewnij siê, ¿e widget zosta³ poprawnie utworzony
	if (!GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("UEnemyUIComponent: Widget not assigned!"));
	}
}

void UEnemyHealthBarComponent::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (UUserWidget* userWidget = GetWidget())
	{
		if (UProgressBar* HPBar = Cast<UProgressBar>(userWidget->GetWidgetFromName(TEXT("Progress_HP"))))
		{
			HPBar->SetPercent(CurrentHealth / MaxHealth);
		}
	}
}