// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpawnPoint.h"

// Sets default values
AMySpawnPoint::AMySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPointTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnPointTrigger"));
	check(SpawnPointTrigger != nullptr);
	SpawnPointTrigger->SetupAttachment(RootComponent); //设置触发器为根组件的子组件)
	SpawnPointTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f)); //设置触发器的大小)

	SpawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPointMesh"));
	SpawnPointMesh->SetupAttachment(SpawnPointTrigger); //设置网格为触发器的子组件
	SpawnPointMesh->SetVisibility(true); //设置网格可见
}

// Called when the game starts or when spawned
void AMySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySpawnPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
	{
		if (AController* Controller = OtherCharacter->GetController())
		{
			AMyPlayerState* ST = Controller->GetPlayerState<AMyPlayerState>();
			FTransform SpawnPoint = FTransform(OtherCharacter->GetActorRotation(), GetActorLocation(), FVector(1.0f, 1.0f, 1.0f));
			ST->SetSpawnPoint(SpawnPoint);
			SpawnPointMesh->SetVisibility(false); //当玩家进入生成点时隐藏生成点的可视化组件
		}
    }
	

}

