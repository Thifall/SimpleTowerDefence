// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Towers/TowerBase.h"
#include "ProjectileTower.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMPLETOWERDEFENCE_API AProjectileTower : public ATowerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tower")
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;

protected:	
	AEnemyBase* _currentTarget = nullptr;

	virtual void OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ClearIfWasCurrentTarget(AActor* OtherActor);
	virtual void OnTargetDestroyed(AActor* DestroyedActor);
	virtual void Tick(float DeltaTime) override;
	void TryAttackTarget(float DeltaTime);
	bool EnsureHasTarget();
	void ShootProjectileAtTarget();
};
