#include "Level/LevelPath.h"
#include "Kismet/GameplayStatics.h"

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
	SpawnEnemy(DeltaTime);
}

void ALevelPath::SpawnEnemy(float deltaTime)
{
	if (_spawnCooldown <= 0.0f && EnemiesToSpawn > 0)
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

		_spawnCooldown = SpawnInterval;
		EnemiesToSpawn--;
	}
	else
	{
		_spawnCooldown -= deltaTime;
	}
}