// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <EnemyBase.h>
#include <Components/BoxComponent.h>
#include <ProjectileBase.h>
#include "TowerBase.generated.h"

UCLASS(Blueprintable)
class SIMPLETOWERDEFENCE_API ATowerBase : public AActor
{
	GENERATED_BODY()

public:
	ATowerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	float AttackSpeed = 1.f; // Attacks per second

	UPROPERTY(EditDefaultsOnly, Category = "Tower")
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr; 

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RootMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* EnemyDetection = nullptr;;

protected:
	AEnemyBase* _currentTarget = nullptr;
	TArray<AEnemyBase*> _enemiesInRange;
	float _timeSinceLastAttack = 0.f;

public:

	UFUNCTION()
	void OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AttackTarget(float DeltaTime);
	bool EnsureHasTarget();
	bool IsShootCooldownReady();
	void ShootProjectileAtTarget();
};
