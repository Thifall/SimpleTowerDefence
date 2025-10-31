// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/CapsuleComponent.h"
#include "UI/Enemies/EnemyHealthBarComponent.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedSignature, class AEnemyBase*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDamagedSignature, float, Damage, float, RemainingHealth);

/**
 * Base class for all enemies in the game.
 * Provides path following, health management and damage handling.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class SIMPLETOWERDEFENCE_API AEnemyBase : public AActor
{
    GENERATED_BODY()

public:
    AEnemyBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|UI")
    UEnemyHealthBarComponent* EnemyUI;

    /** Called when enemy dies (health reaches 0) */
    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyDiedSignature OnEnemyDied;

    /** Called when enemy takes damage but survives */
    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyDamagedSignature OnEnemyDamaged;

    /** Sets the path for this enemy to follow */
    UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
    void SetPath(UPathComponent* path);

    /** Movement speed in units per second */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float MovementSpeed = 200.f;

    /** Maximum health points */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float MaxHealth = 100.f;

    /** Current health points */
    UPROPERTY(BlueprintReadOnly, Category = "Enemy|Stats", meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    /** Get current health as percentage (0-1) */
    UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
    float GetHealthPercentage() const { return CurrentHealth / FMath::Max(MaxHealth, 1.0f); }

    /** Returns true if enemy is alive (health > 0) */
    UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
    bool IsAlive() const { return CurrentHealth > 0.0f; }

    /** Override to handle incoming damage. Return final damage dealt. */
    virtual float InflictDamage(float DamageAmount, AActor* DamageCauser);

protected:
    /** Physical collision for this enemy */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CapsuleComponent;

    /** Visual representation of this enemy */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    /** Component holding path information */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    UPathComponent* PathComponent;

    /** Distance to consider waypoint reached */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float ReachThreshold = 5.0f;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    /** Called when health reaches 0. Override in blueprints for death effects */
    UFUNCTION(BlueprintNativeEvent, Category = "Enemy|Events")
    void OnDeath();

    /** Called after taking damage but before dying. Override in blueprints for damage effects */
    UFUNCTION(BlueprintNativeEvent, Category = "Enemy|Events")
    void OnDamaged(float DamageAmount, float HealthRemaining);

    /** Called when reaching end of path. Override in blueprints for custom behavior */
    UFUNCTION(BlueprintNativeEvent, Category = "Enemy|Events") 
    void OnReachedEnd();

private:
    int32 CurrentWaypointIndex = 0;
    void MoveAlongPath(float DeltaTime);
    void UpdateMovement(float DeltaTime);
    bool IsAtFinalWaypoint() const;
};