// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <PathComponent.h>
#include "EnemyBase.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Path")
	void SetPath(UPathComponent* path);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MovementSpeed = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int HitPoints = 100;

	AEnemyBase();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	UPathComponent* PathComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	int _currentPathIndex = 0;
	float _reachThreshold = 1.0f;
	void MoveAlongPath(float DeltaTime);
};
