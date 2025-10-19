// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBase.h"
#include <Kismet/GameplayStatics.h>
#include <ProjectileBase.h>
#include "Components/BoxComponent.h"

// Sets default values
ATowerBase::ATowerBase()
{
	RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMeshComponent);

	EnemyDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyDetection"));
	EnemyDetection->SetupAttachment(RootComponent);
	EnemyDetection->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemyDetection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyDetection->SetGenerateOverlapEvents(true);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_timeSinceLastAttack = AttackSpeed;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyDetection->OnComponentBeginOverlap.AddDynamic(this, &ATowerBase::OnEnemyBeginOverlap);
	EnemyDetection->OnComponentEndOverlap.AddDynamic(this, &ATowerBase::OnEnemyEndOverlap);
	return;
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackTarget(DeltaTime);
}

void ATowerBase::OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	_enemiesInRange.AddUnique(Cast<AEnemyBase>(OtherActor));
	if (!_currentTarget)
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
		{
			_currentTarget = Enemy;
			// Subscribe to enemy's destruction event
			_currentTarget->OnDestroyed.AddDynamic(this, &ATowerBase::OnTargetDestroyed);
		}
	}
}

void ATowerBase::OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (_currentTarget && OtherActor == _currentTarget)
	{
		_currentTarget->OnDestroyed.RemoveDynamic(this, &ATowerBase::OnTargetDestroyed);
		_currentTarget = nullptr;
	}
	_enemiesInRange.Remove(Cast<AEnemyBase>(OtherActor));
}

void ATowerBase::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (_currentTarget && DestroyedActor == _currentTarget)
	{
		_currentTarget = nullptr;
	}
	_enemiesInRange.Remove(Cast<AEnemyBase>(DestroyedActor));
}

void ATowerBase::AttackTarget(float deltaTime)
{
	_timeSinceLastAttack += deltaTime;

	if (!EnsureHasTarget())
	{
		// No target available
		return;
	}

	if (!IsShootCooldownReady())
	{
		// Still in cooldown
		return;
	}
	ShootProjectileAtTarget();
}

bool ATowerBase::IsShootCooldownReady()
{
	return _timeSinceLastAttack > (1 / AttackSpeed);
}

bool ATowerBase::EnsureHasTarget()
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

void ATowerBase::ShootProjectileAtTarget()
{
	if (!ProjectileClass || !_currentTarget)
	{
		return;
	}
	_timeSinceLastAttack = 0.f;

	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = (_currentTarget->GetActorLocation() - GetActorLocation()).Rotation();
	FTransform spawnTransform(spawnRotation, spawnLocation);
	AProjectileBase* spawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (spawnedProjectile)
	{
		spawnedProjectile->SetTarget(_currentTarget);
		UGameplayStatics::FinishSpawningActor(spawnedProjectile, spawnTransform);
	}
}

