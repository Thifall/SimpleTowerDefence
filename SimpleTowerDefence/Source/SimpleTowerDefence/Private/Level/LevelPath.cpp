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
	TrySpawnNextEnemy(DeltaTime);
}

void ALevelPath::TrySpawnNextEnemy(float deltaTime)
{
	_spawnCooldown += deltaTime;
	if (_currentWaveIndex >= EnemyWaves.Num()) //nie ma wiêcej fal
	{
		// tu mo¿na dodaæ trigger eventu zakoñczenia wszystkich fal (np. zwyciêstwo gracza, nastêpny poziom z inn¹ œcie¿k¹ etc)
		return;
	}

	if(EnemyWaves[_currentWaveIndex].EnemyList.Num() == 0) //Empty wave, skip
	{
		//to siê nie powinno zdarzyæ, byæ mo¿e warto dodaæ logowanie b³êdu
		return;
	}

	if (EnemyWaves[_currentWaveIndex].EnemyList.Num() <= _currentEnemyIndex) //fala zakoñczona
	{
		return;
	}

	if(EnemyWaves[_currentWaveIndex].SpawnInterval > _spawnCooldown)
	{
		//jeszcze nie czas na kolejnego przeciwnika
		return;
	}

	UPathComponent* path = NewObject<UPathComponent>(this);
	path->PathNodes.Empty();

	for (ATargetPoint* targetPoint : PathNodes)
	{
		path->PathNodes.Add(targetPoint);
	}

	FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
	TSubclassOf<AEnemyBase> enemyClassToSpawn = EnemyWaves[_currentWaveIndex].EnemyList[_currentEnemyIndex];
	AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemyBase>(enemyClassToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (SpawnedEnemy)
	{
		SpawnedEnemy->SetPath(path);
		UGameplayStatics::FinishSpawningActor(SpawnedEnemy, SpawnTransform);
	}

	_spawnCooldown = 0.0f;
	_currentEnemyIndex++;
}

void ALevelPath::StartNextWave()
{
	_currentEnemyIndex = 0;
	_spawnCooldown = 0.0f;
	_currentWaveIndex++;
}