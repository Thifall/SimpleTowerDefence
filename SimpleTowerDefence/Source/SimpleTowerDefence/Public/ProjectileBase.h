// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <EnemyBase.h>
#include "ProjectileBase.generated.h"

UCLASS(Blueprintable, Abstract)
class SIMPLETOWERDEFENCE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	UFUNCTION(BlueprintCallable)
	virtual void SetTarget(AEnemyBase* target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed = 600.f; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
