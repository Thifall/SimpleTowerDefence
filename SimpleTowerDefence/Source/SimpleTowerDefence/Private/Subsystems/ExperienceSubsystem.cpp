// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ExperienceSubsystem.h"

UExperienceSubsystem::UExperienceSubsystem()
{
	//prosty exp table
	_experienceTable.Add(1, 100);
	_experienceTable.Add(2, 120);
	_experienceTable.Add(3, 150);
	_experienceTable.Add(4, 220);
	_experienceTable.Add(5, 270);
	// pewnie trzeba by zarychtowaæ jakiœ pliczek ;)
}

void UExperienceSubsystem::AddExperiencePoints(int experiencePoints)
{
	if (GetExperienceForNextLevel() < 0) 
	{
		return; // maksymalny poziom osi¹gniêty
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Dodano %d punktow doswiadczenia."), experiencePoints));
	}
	_currentExperience += experiencePoints;
	OnExperienceGained.Broadcast(experiencePoints);
	// SprawdŸ, czy gracz awansowa³ na wy¿szy poziom
	while (_experienceTable.Contains(_currentLevel) && _currentExperience >= _experienceTable[_currentLevel])
	{
		_currentExperience -= _experienceTable[_currentLevel];
		_currentLevel++;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Awansowano na poziom %d!"), _currentLevel));
		}
		int requiredExpForNextLevel = GetExperienceForNextLevel();
		OnLevelUp.Broadcast(_currentLevel, requiredExpForNextLevel);
	}
}

int UExperienceSubsystem::GetExperienceForNextLevel()
{
	if (_experienceTable.Contains(_currentLevel))
	{
		return _experienceTable[_currentLevel];
	}
	return -1; // Maksymalny poziom osi¹gniêty
}