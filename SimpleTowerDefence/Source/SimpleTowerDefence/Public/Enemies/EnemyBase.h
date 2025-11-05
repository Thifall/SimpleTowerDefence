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

    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyDiedSignature OnEnemyDied;

    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyDamagedSignature OnEnemyDamaged;

    UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
    void SetPath(UPathComponent* path);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float MovementSpeed = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Stats")
    int ExperienceReward = 20;

    UPROPERTY(BlueprintReadOnly, Category = "Enemy|Stats", meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    /** Get current health as percentage (0-1) */
    UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
    float GetHealthPercentage() const { return CurrentHealth / FMath::Max(MaxHealth, 1.0f); }

    UFUNCTION(BlueprintPure, Category = "Enemy|Stats")
    bool IsAlive() const { return CurrentHealth > 0.0f; }

    virtual float InflictDamage(float DamageAmount, AActor* DamageCauser);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    UPathComponent* PathComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float ReachThreshold = 5.0f;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintNativeEvent)
    void OnDeath();

    UFUNCTION(BlueprintNativeEvent) 
    void OnReachedEnd();

private:
    int32 CurrentWaypointIndex = 0;
    void MoveAlongPath(float DeltaTime);
    void UpdateMovement(float DeltaTime);
    bool IsAtFinalWaypoint() const;
};