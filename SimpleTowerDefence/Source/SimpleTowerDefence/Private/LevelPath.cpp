// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPath.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"

ALevelPath::ALevelPath()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelPath::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpawnCooldown <= 0.0f && EnemiesToSpawn > 0)
	{
		UPathComponent* path = NewObject<UPathComponent>(this);

		path->PathNodes.Empty();
		for (ATargetPoint* targetPoint : PathNodes)
		{
			path->PathNodes.Add(targetPoint);
		}

		FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
		AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemyBase>(EnemyClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (SpawnedEnemy)
		{
			SpawnedEnemy->SetPath(path);
			UGameplayStatics::FinishSpawningActor(SpawnedEnemy, SpawnTransform);
		}
		SpawnCooldown = SpawnInterval;
		EnemiesToSpawn--;
	}
	else
	{
		SpawnCooldown -= DeltaTime;
	}
}


