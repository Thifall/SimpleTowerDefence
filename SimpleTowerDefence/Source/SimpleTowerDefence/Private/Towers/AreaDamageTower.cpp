// Fill out your copyright notice in the Description page of Project Settings.


#include "Towers/AreaDamageTower.h"

void AAreaDamageTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DealAreaDamage();
}

void AAreaDamageTower::DealAreaDamage()
{
	if (!IsAttackCooldownReady())
	{
		//cd nie gotowy
		return;
	}

	//atakujemy wszystkich w zasiêgu
	for (AEnemyBase* enemy : _enemiesInRange)
	{
		enemy->InflictDamage(BaseDamage, this);
	}
	if (AuraMaterialInstance && _enemiesInRange.Num() > 0) //efekt wizualny tylko wtedy, gdy rzeczywiœcie atakujemy wrogów
	{
		double LastAttackTime = GetWorld()->GetTimeSeconds();
		AuraMaterialInstance->SetScalarParameterValue(TEXT("AttackTime"), LastAttackTime);
	}
	//reset cd
	_timeSinceLastAttack = 0.f;
}

