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
}