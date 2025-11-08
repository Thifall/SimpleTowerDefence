#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "EnemyWaveDetails.generated.h"


USTRUCT(BlueprintType)
struct SIMPLETOWERDEFENCE_API FEnemyWaveDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TArray<TSubclassOf<AEnemyBase>> EnemyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float SpawnInterval = 1.0f;
};