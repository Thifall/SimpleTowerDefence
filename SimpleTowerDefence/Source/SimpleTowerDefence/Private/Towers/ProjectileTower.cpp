// Fill out your copyright notice in the Description page of Project Settings.

#include "Towers/ProjectileTower.h"

void AProjectileTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryAttackTarget(DeltaTime);
}

void AProjectileTower::OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnEnemyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//próbujemy ustawiæ cel, jeœli go nie mamy a w zasiêgu s¹ przeciwnicy
	if (!_currentTarget && _enemiesInRange.Num() > 0)
	{
		//zak³adamy, ¿e pierwszy na liœcie jest najdalej posuniêty, co nie musi byæ prawd¹, w przypadku 
		//przeciwników o ró¿nych prêdkoœciach ruchu, ale na ten moment jest ok
		_currentTarget = _enemiesInRange[0];
	}
}

void AProjectileTower::OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEnemyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//jeœli to by³ nasz cel, to go czyœcimy
	ClearIfWasCurrentTarget(OtherActor);
}

void AProjectileTower::OnTargetDestroyed(AActor* DestroyedActor)
{
	Super::OnTargetDestroyed(DestroyedActor);
	//jeœli to by³ nasz cel, to go czyœcimy
	if (_currentTarget && DestroyedActor == _currentTarget)
	{
		_currentTarget = nullptr;
	}
}

void AProjectileTower::ClearIfWasCurrentTarget(AActor* OtherActor)
{
	if (_currentTarget && OtherActor == _currentTarget)
	{
		_currentTarget = nullptr;
	}
}

void AProjectileTower::TryAttackTarget(float deltaTime)
{
	if (!EnsureHasTarget())
	{
		//nie mamy celu
		return;
	}

	if (!IsAttackCooldownReady())
	{
		//dalej na cd
		return;
	}
	ShootProjectileAtTarget();
}

bool AProjectileTower::EnsureHasTarget()
{
	bool hasTarget = false;
	if (!_currentTarget)
	{
		if (_enemiesInRange.Num() > 0)
		{
			_currentTarget = _enemiesInRange[0];
			hasTarget = true;
		}
	}
	else
	{
		hasTarget = true;
	}
	return hasTarget;
}

void AProjectileTower::ShootProjectileAtTarget()
{
	if (!ProjectileClass || !_currentTarget)
	{
		return;
	}
	_timeSinceLastAttack = 0.f;

	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = (_currentTarget->GetActorLocation() - GetActorLocation()).Rotation();
	FTransform spawnTransform(spawnRotation, spawnLocation);

	if (AProjectileBase* spawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass,
		spawnLocation,
		spawnRotation,
		FActorSpawnParameters()))
	{
		spawnedProjectile->Damage = BaseDamage;
		spawnedProjectile->SetTarget(_currentTarget);
	}
}