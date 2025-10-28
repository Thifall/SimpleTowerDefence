// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/SimpleProjectile.h"
#include "Kismet/GameplayStatics.h"

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

    // If we have a valid target, check distance and apply damage when in range
    if (Target && IsValid(Target))
    {
        float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
        if (Distance <= HitDistance)
        {
            // Apply damage using the projectile's Damage property
            UGameplayStatics::ApplyDamage(Target, Damage, GetInstigatorController(), this, nullptr);

            // Destroy the projectile after dealing damage
            Destroy();
        }
    }
    else
    {
        // No target - destroy to avoid stray projectiles
        Destroy();
    }
}