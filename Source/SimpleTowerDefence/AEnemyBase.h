// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "AEnemyBase.generated.h"

UCLASS()
class SIMPLETOWERDEFENCE_API AAEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* RootMeshComponent;

	UPROPERTY(EditAnywhere)
	int Health = 100;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 600;

	UPROPERTY(EditAnywhere)
	int CurrentNodeIndex;

	UPROPERTY(EditInstanceOnly);
	ATargetPoint* CurrentTargetNode[5];

	UFUNCTION(BlueprintCallable)
	void MoveTowardsCurrentTargetNode();

};
