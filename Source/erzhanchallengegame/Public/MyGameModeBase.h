// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayerState.h"
#include "MyGameModeBase.generated.h"

class AThirdCharacter;
/**
 * 
 */
UCLASS()
class ERZHANCHALLENGEGAME_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMyGameModeBase();

	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AThirdCharacter> ActiveCharacter;

	UPROPERTY()
	TObjectPtr<AThirdCharacter> PooledCharacter;

	UFUNCTION()
	void SwapCharacter(APlayerController* PC);

	virtual void RestartPlayer(AController* NewPlayer) override;

	UPROPERTY()
	bool bIsSwaped = false;

	UFUNCTION()
	void ResetSwap()
	{
		bIsSwaped = false;
	}
};
