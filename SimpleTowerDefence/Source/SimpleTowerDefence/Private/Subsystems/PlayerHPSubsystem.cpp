// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/PlayerHPSubsystem.h"

void UPlayerHPSubsystem::DecreaseHealth(int32 Amount)
{
	if (CurrentHealth <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0, StartingHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Gracz straci³ %d punktow zdrowia. Pozosta³o: %d"), Amount, CurrentHealth));
	}
	if (CurrentHealth == 0)
	{
		OnPlayerDefeated.Broadcast();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gracz zosta³ pokonany!"));
		}
	}
}