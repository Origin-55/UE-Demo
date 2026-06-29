// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyPlayerState.h"
#include "GameFramework/Character.h"
#include "MySpawnPoint.generated.h"

UCLASS()
class ERZHANCHALLENGEGAME_API AMySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Spawnpoint)
	TObjectPtr<UStaticMeshComponent> SpawnPointMesh; //生成点的可视化组件

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Spawnpoint)
	TObjectPtr<UBoxComponent> SpawnPointTrigger; //生成点的触发器组件



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //重叠事件函数)
};
