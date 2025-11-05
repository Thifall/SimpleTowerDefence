// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerHpInterface.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Border.h"
#include <Components/SizeBox.h>

void UPlayerHpInterface::NativeConstruct()
{
	Super::NativeConstruct();
	if (UWorld* World = GetWorld())
	{
		PlayerHpSubsystem = World->GetSubsystem<UPlayerHPSubsystem>();
	}

	if (!PlayerHpSubsystem || !ContainerBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerHpSubsystem is null in PlayerHpInterface"));
		return;
	}

	CreateHealthBlocks(PlayerHpSubsystem->StartingHealth);
	UpdateHealthInterface(PlayerHpSubsystem->CurrentHealth);
	PlayerHpSubsystem->OnHealthChanged.AddDynamic(this, &UPlayerHpInterface::HandleHealthChanged);
}

void UPlayerHpInterface::NativeDestruct()
{
	Super::NativeDestruct();
	if (PlayerHpSubsystem)
	{
		PlayerHpSubsystem->OnHealthChanged.RemoveDynamic(this, &UPlayerHpInterface::HandleHealthChanged);
	}
}

void UPlayerHpInterface::CreateHealthBlocks(int startingHealth)
{
	ContainerBox->ClearChildren();
	HpBars.Empty();
	const float BlockWidth = 100.0f;
	const float BlockHeight = 30.0f;
	for (int i = 0; i < startingHealth; i++)
	{
		USizeBox* size = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		size->SetWidthOverride(BlockWidth);
		size->SetHeightOverride(BlockHeight);

		UBorder* hpBar = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		hpBar->SetBrushColor(ActiveBarColor);

		size->AddChild(hpBar);
		if (UHorizontalBoxSlot* slot = ContainerBox->AddChildToHorizontalBox(size))
		{
			slot->SetPadding(FMargin(1.0f, 0.0f));
			slot->SetVerticalAlignment(VAlign_Center);
			slot->SetHorizontalAlignment(HAlign_Left);
		}
		HpBars.Add(hpBar);
	}
}

void UPlayerHpInterface::UpdateHealthInterface(int currentHealth)
{
	for (int i = 0; i < HpBars.Num(); i++)
	{
		const bool isActive = i < currentHealth;
		HpBars[i]->SetBrushColor(isActive ? ActiveBarColor : InactiveBarColor);
	}
}

void UPlayerHpInterface::HandleHealthChanged(int currentHealth)
{
	UpdateHealthInterface(currentHealth);
}