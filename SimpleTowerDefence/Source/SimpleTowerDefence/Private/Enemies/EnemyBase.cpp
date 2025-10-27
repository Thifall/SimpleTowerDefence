#include "Enemies/EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyBase::SetPath(UPathComponent* path)
{
	PathComponent = path;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveAlongPath(DeltaTime);
}

void AEnemyBase::MoveAlongPath(float DeltaTime)
{
	if (!PathComponent || PathComponent->PathNodes.Num() == 0)
	{
		return;
	}

	if (!PathComponent->PathNodes.IsValidIndex(_currentPathIndex))
	{
		return;
	}

	AActor* currentTargetNode = PathComponent->PathNodes[_currentPathIndex];
	FVector targetLocation = currentTargetNode->GetActorLocation();
	FVector currentLocation = GetActorLocation();

	float distanceToTarget = (currentTargetNode->GetActorLocation() - GetActorLocation()).Size();

	if (distanceToTarget < _reachThreshold)
	{
		_currentPathIndex++;
		if (_currentPathIndex >= PathComponent->PathNodes.Num())
		{
			Destroy();
			return;
		}
	}
	else
	{
		FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();
		float step = MovementSpeed * DeltaTime;
		SetActorLocation(currentLocation + direction * FMath::Min(step, distanceToTarget));
	}
}