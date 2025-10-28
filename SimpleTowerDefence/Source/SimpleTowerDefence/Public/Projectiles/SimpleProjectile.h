// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "SimpleProjectile.generated.h"

/**
 * Basic projectile implementation with straight-line movement to target
 */
UCLASS(Blueprintable)
class SIMPLETOWERDEFENCE_API ASimpleProjectile : public AProjectileBase
{
    GENERATED_BODY()

public:
    ASimpleProjectile();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Distance within which the projectile will be considered to have hit the target */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float HitDistance = 20.f;
};