// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "Enemies/EnemyBase.h"
#include "PathComponent.h"
#include "EnemyWaveDetails.h"
#include "LevelPath.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveCompletedSignature);

UCLASS()
class SIMPLETOWERDEFENCE_API ALevelPath : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Level|Path")
	TArray<ATargetPoint*> PathNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Enemies")
	TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Enemies")
	TArray<FEnemyWaveDetails> EnemyWaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Enemies")
	float SpawnInterval = 2.0f;

	UFUNCTION(BlueprintCallable, Category = "Level|Path")
	void StartNextWave();

	UPROPERTY(BlueprintAssignable, Category = "Level|Path")
	FOnWaveCompletedSignature OnWaveCompleted;

	ALevelPath();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//need to handle enemy destroyed to remove from _spawnedEnemies
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

private:
	float _spawnCooldown = 0.0f;
	int _currentWaveIndex = 0;
	int _currentEnemyIndex = 0;
	TArray<AEnemyBase*> _spawnedEnemies;

	void TrySpawnNextEnemy(float deltaTime);
};