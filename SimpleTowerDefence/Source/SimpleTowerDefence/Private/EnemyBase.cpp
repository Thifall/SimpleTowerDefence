// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

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
	if (!PathComponent || PathComponent->PathNodes.Num() == 0)
	{
		return;
	}
	FVector targetLocation = PathComponent->PathNodes[CurrentPathIndex]->GetActorLocation();
	FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();

	FVector newLocation = GetActorLocation() + direction * MovementSpeed * DeltaTime;
	SetActorLocation(newLocation);

	if (FVector::Dist(GetActorLocation(), targetLocation) <= 1.0f) // Threshold to consider "reached"
	{
		CurrentPathIndex++;
		if (CurrentPathIndex >= PathComponent->PathNodes.Num())
		{
			Destroy();
		}
	}
}

