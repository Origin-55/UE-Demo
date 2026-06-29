// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdCharacter.h"

// Sets default values
AThirdCharacter::AThirdCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SpringArm3 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm3")); 
	check(SpringArm3 != nullptr);
	SpringArm3->SetupAttachment(GetMesh());
	SpringArm3->TargetArmLength = ArmLength;
	SpringArm3->TargetOffset = SpringArmOffset;
	SpringArm3->bDoCollisionTest = true; // 碰撞检测以防止穿模
	SpringArm3->bUsePawnControlRotation = true;
	

	Camera3 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3")); 
	check(Camera3 != nullptr);
	Camera3->SetupAttachment(SpringArm3);
	Camera3->FieldOfView = ThirdPersonFieldOfView;
	Camera3->bUsePawnControlRotation = false;

	
}

// Called when the game starts or when spawned
void AThirdCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 在 BeginPlay 中捕获 Mesh 相对 Transform，确保已应用蓝图中的修改（旋转、偏移等）
	InitMeshRelativeTransform = GetMesh()->GetRelativeTransform();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}
	if (ThirdPersonAnim)
	{
		GetMesh()->SetAnimInstanceClass(ThirdPersonAnim);
	}
}

// Called every frame
void AThirdCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isDead && GetActorLocation().Z <= 470.0f)
	{
		Death_Implementation();
	}

}

// Called to bind functionality to input
void AThirdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdCharacter::Look);
	}

}

void AThirdCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D val = Value.Get<FVector2D>();
	if (!Controller)return;
	const FRotator Rotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y),val.Y);
	AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X), val.X);
}

void AThirdCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D val = Value.Get<FVector2D>();
	if (!Controller)return;
	AddControllerPitchInput(val.Y);
	AddControllerYawInput(val.X);
}

void AThirdCharacter::Death_Implementation()
{
	if (isDead)return;
	isDead = true;
	GetMesh()->SetSimulatePhysics(true);
	SpringArm3->bDoCollisionTest = false;
	if (auto* CharMove = GetCharacterMovement())
	{
		CharMove->SetMovementMode(EMovementMode::MOVE_None);
	}
	if (APlayerController* PC = Cast<APlayerController>(Controller))
		DisableInput(PC);

	GetWorld()->GetAuthGameMode<AMyGameModeBase>()->ResetSwap();

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&AThirdCharacter::Respawn,
		3.0f,
		false
	);
}

void AThirdCharacter::Respawn()
{
	APlayerController* PC = GetController<APlayerController>();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Respawn: Controller is null, aborting respawn."));
		return;
	}
	GetWorld()->GetAuthGameMode<AMyGameModeBase>()->SwapCharacter(PC);
}

void AThirdCharacter::Deactivate()
{
	// 停用前先清理角色状态：停止物理模拟、重置骨骼姿态，防止池中角色保持 Ragdoll 姿势
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetRelativeTransform(InitMeshRelativeTransform);
	GetMesh()->InitAnim(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AThirdCharacter::Activate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}
	
void AThirdCharacter::ResetCharacter()
{
	GetMesh()->SetSimulatePhysics(false);
	// 关键修复：确保所有骨骼体的物理模拟都已停止，
	// 并强制骨骼姿态回到动画蓝图的控制�?
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	// 重置 Mesh 相对 Transform 到初始状态（消除 ragdoll 造成的位移）
	GetMesh()->SetRelativeTransform(InitMeshRelativeTransform);
	// 重新初始化动画实例，强制骨骼回到参考姿势，
	// 解决重生后模型保�?ragdoll 倒地姿势的问�?
	GetMesh()->InitAnim(true);

	SpringArm3->bDoCollisionTest = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	isDead = false;

}

void AThirdCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}
}

void AThirdCharacter::UnPossessed()
{
	Super::UnPossessed();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ThirdPersonContext);
		}
	}
}

