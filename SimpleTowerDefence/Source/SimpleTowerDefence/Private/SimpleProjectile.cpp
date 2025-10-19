// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleProjectile.h"

ASimpleProjectile::ASimpleProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ASimpleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTowardsTarget(DeltaTime);
}

void ASimpleProjectile::MoveTowardsTarget(float DeltaTime)
{
	if (Target)
	{
		FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
		SetActorLocation(NewLocation);
		// Check if the projectile has reached the target
		if (FVector::Dist(NewLocation, Target->GetActorLocation()) < 10.f) // Assuming 10 units is close enough
		{
			// Here you can add logic for what happens when the projectile hits the target
			Destroy(); // Destroy the projectile for now
		}
	}
	else
	{
		Destroy(); // Destroy the projectile if there is no target
	}
}

void ASimpleProjectile::SetTarget(AEnemyBase* target)
{
	Target = target;
}