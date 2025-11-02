// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemies/EnemyBase.h"
#include "Components/SphereComponent.h"
#include "Projectiles/ProjectileBase.h"
#include "TowerBase.generated.h"

UCLASS(Abstract)
class SIMPLETOWERDEFENCE_API ATowerBase : public AActor
{
	GENERATED_BODY()

public:
	ATowerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	float AttackSpeed = 1.f; // Attacks per second

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RootMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* EnemyDetection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	UStaticMeshComponent* AuraMesh;

	UMaterialInstanceDynamic* AuraMaterialInstance = nullptr;	
protected:
	float _timeSinceLastAttack = 0.f;
	TArray<AEnemyBase*> _enemiesInRange;

	UFUNCTION()
	virtual void OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnTargetDestroyed(AActor* DestroyedActor);


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool IsAttackCooldownReady();
};