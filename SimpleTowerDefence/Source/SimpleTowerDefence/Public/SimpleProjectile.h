// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SimpleProjectile.generated.h"

/**
 * 
 */
UCLASS(blueprintable)
class SIMPLETOWERDEFENCE_API ASimpleProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ASimpleProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	AEnemyBase* Target;

	void SetTarget(AEnemyBase* target) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void MoveTowardsTarget(float DeltaTime);	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
