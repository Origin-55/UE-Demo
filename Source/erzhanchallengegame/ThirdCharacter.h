// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Gameframework/SpringArmComponent.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Math/RotationMatrix.h"
#include "TimerManager.h"
#include "MyPlayerState.h"
#include "GameFramework/GameMode.h"
#include "BPI_Death.h"
#include "MyGameModeBase.h"
#include "ThirdCharacter.generated.h"

class UInputMappingContext;
class UinputAction;
class UAnimInstance;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class ERZHANCHALLENGEGAME_API AThirdCharacter : public ACharacter, public IBPI_Death
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdCharacter();

	bool isDead = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> ThirdPersonContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ArmLength = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector SpringArmOffset = FVector(0.0f, 0.0f, 200.0f);

	



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TSubclassOf<UAnimInstance> ThirdPersonAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ThirdPersonFieldOfView = 90.0f;

	

	virtual void Death_Implementation() override;

	UFUNCTION()
	void Respawn();

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void Deactivate();

	UFUNCTION()
	void ResetCharacter();

	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void UnPossessed() override;

	UPROPERTY()
	FTransform InitMeshRelativeTransform;

};