// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BPI_Death.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBPI_Death : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ERZHANCHALLENGEGAME_API IBPI_Death
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void Death();

	virtual void Death_Implementation() {};
};
