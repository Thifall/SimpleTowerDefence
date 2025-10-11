// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <PathComponent.h>
#include <EnemyBase.h>
#include "LevelPath.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API ALevelPath : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Path")
	TArray<ATargetPoint*> PathNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float SpawnInterval = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int EnemiesToSpawn = 5;

	ALevelPath();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float SpawnCooldown = 0.0f;
	
};
