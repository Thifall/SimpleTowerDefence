// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Towers/TowerBase.h"
#include "AreaDamageTower.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMPLETOWERDEFENCE_API AAreaDamageTower : public ATowerBase
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float DeltaTime) override;
	void DealAreaDamage();
};
