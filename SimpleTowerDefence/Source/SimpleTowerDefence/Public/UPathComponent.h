// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/TargetPoint.h>
#include "UPathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLETOWERDEFENCE_API UUPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPROPERTY(EditInstanceOnly, Category = "Path")
	TArray<ATargetPoint*> PathArray;
		
};
