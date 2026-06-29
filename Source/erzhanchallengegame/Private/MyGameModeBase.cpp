// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "ThirdCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = nullptr;           // 不让引擎有任何 Pawn 可以自动生成
	
}


void AMyGameModeBase::RestartPlayer(AController* NewPlayer)
{

}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
    
	UClass* PawnClass = GetDefaultPawnClassForController(PC);
	if (!PawnClass)
	{
		PawnClass = AThirdCharacter::StaticClass(); // 保底
	}

	if (PC!=nullptr)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->EnableInput(PC);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform SpawnTransform = FindPlayerStart(PC)->GetActorTransform();
		if (!SpawnTransform.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid spawn point for the ActiveCharacter."));
			return;
		}
		ActiveCharacter = GetWorld()->SpawnActor<AThirdCharacter>(PawnClass, SpawnTransform, SpawnParams);
		check(ActiveCharacter != nullptr);
		PC->Possess(ActiveCharacter.Get());
		PC->SetViewTarget(ActiveCharacter.Get());
		// Store the initial spawn transform into the player state so future respawns use a valid location
		if (AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>())
		{
			PS->SetSpawnPoint(SpawnTransform);
		}
		PooledCharacter = GetWorld()->SpawnActor<AThirdCharacter>(PawnClass, SpawnTransform, SpawnParams);
		check(PooledCharacter != nullptr);
		PooledCharacter->Deactivate();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerController found to possess the ActiveCharacter."));
	}


	


}

void AMyGameModeBase::SwapCharacter(APlayerController *PC)
{
	if(bIsSwaped)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("multiple swap detected, skipping SwapCharacter call."));
		return;
	}
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("SwapCharacter: PC is null."));
		return;
	}

	if (!PooledCharacter || !ActiveCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("SwapCharacter: ActiveCharacter or PooledCharacter is null."));
		return;
	}
	// Try to use the player state's saved spawn point first
	FTransform Spawn = FTransform::Identity;
	if (AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>())
	{
		Spawn = PS->GetSpawnPoint();
	}

	if (!Spawn.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapCharacter: PlayerState spawn point is invalid, trying to find a player start."));
		Spawn = FindPlayerStart(PC)->GetActorTransform();
	}

	// 关键修复：必须先退出 Ragdoll（停止物理模拟），再设置 Transform
	// 否则 ResetCharacter 退出 Ragdoll 时会根据 Mesh 的物理位置重新定位 Capsule，
	// 覆盖掉刚刚 SetActorTransform 设置的正确复活点位置。
	PooledCharacter->ResetCharacter();
	PooledCharacter->SetActorTransform(Spawn);// 确保角色朝向正确
	// 安全保护：SetActorTransform 后再次确保 Mesh 相对 Transform 与蓝图一致
	PooledCharacter->GetMesh()->SetRelativeTransform(PooledCharacter->InitMeshRelativeTransform);
	if (UCharacterMovementComponent* CM = PooledCharacter->GetCharacterMovement())
	{
		CM->Velocity = FVector::ZeroVector;
	}
	PooledCharacter->Activate();


	ActiveCharacter->Deactivate();
	// 显式设置控制旋转，确保复活后摄像机朝向与角色一致
	PC->SetControlRotation(Spawn.Rotator());
	PC->Possess(PooledCharacter.Get());
	PooledCharacter->EnableInput(PC);
	PC->SetViewTarget(PooledCharacter.Get());

	if (ActiveCharacter && PooledCharacter)
	{
		bIsSwaped = true;
		AThirdCharacter *Temp = ActiveCharacter;
		ActiveCharacter = PooledCharacter;
		PooledCharacter = Temp;
	}
}
