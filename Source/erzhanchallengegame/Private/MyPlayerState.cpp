// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::SetSpawnPoint(const FTransform& NewSpawnPoint)
{
	SpawnPointTransform = NewSpawnPoint;
}

FTransform AMyPlayerState::GetSpawnPoint()
{
	return SpawnPointTransform;
}