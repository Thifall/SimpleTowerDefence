// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemies/EnemyBase.h"
#include "ProjectileBase.generated.h"

/**
 * Base class for all projectiles in the game.
 * Provides basic straight-line movement towards a target.
 */
UCLASS(Blueprintable, Abstract)
class SIMPLETOWERDEFENCE_API AProjectileBase : public AActor
{
    GENERATED_BODY()
    
public:    
    AProjectileBase();

    /** Sets the target for this projectile */
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    virtual void SetTarget(AEnemyBase* InTarget);

    /** Projectile movement speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed = 600.f;

    /** Damage dealt to target on hit */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Damage = 10.f;

    /** Maximum lifetime of projectile before auto-destruction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float MaxLifeTime = 5.f;

protected:
    /** Current target of the projectile */
    UPROPERTY(Transient)
    AEnemyBase* Target;

    /** Collision sphere for hit detection */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComponent;

    /** Visual representation of the projectile */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ProjectileMesh;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Called when projectile hits something */
    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    bool bHasImpacted = false;
};