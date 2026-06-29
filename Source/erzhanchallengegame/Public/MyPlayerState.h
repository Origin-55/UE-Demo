// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ERZHANCHALLENGEGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = SpawnPoint)
	FTransform GetSpawnPoint();

	UFUNCTION(BlueprintCallable, Category = SpawnPoint)
	void SetSpawnPoint(const FTransform& NewSpawnPoint);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnPoint)
	FTransform SpawnPointTransform;//生成点的变换信息，包括位置、旋转和缩放
	
};
