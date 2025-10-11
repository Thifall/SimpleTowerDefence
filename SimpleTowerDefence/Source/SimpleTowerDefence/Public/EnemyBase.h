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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 0.2f;

	AEnemyBase();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	UPathComponent* PathComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	int CurrentPathIndex = 0;
};
