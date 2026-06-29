// Fill out your copyright notice in the Description page of Project Settings.


#include "MySphere.h"

// Sets default values
AMySphere::AMySphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->InitSphereRadius(160.0f);
	SphereCollision->SetupAttachment(SphereMesh);


	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

}

// Called when the game starts or when spawned
void AMySphere::BeginPlay()
{
	Super::BeginPlay();

	if (TimelineCurve)
	{
		// 绑定曲线回调：
		// 1) UpdateFunctionFloat 在曲线每个采样点被调用，传入当前曲线数值
		// 2) FinishedEvent 在时间轴完成时调用（如果 SetLooping(true) 则不会被调用，除非 SetLooping(false)）
		// 注意：BindDynamic 需要被绑定的成员函数在头文件中用 UFUNCTION() 标记
		UpdateFunctionFloat.BindDynamic(this, &AMySphere::OnTimelineFloatUpdate);
		FinishedEvent.BindDynamic(this, &AMySphere::OnTimelineFinished);

		// 将曲线添加到时间轴并设置完成回调
		Timeline->AddInterpFloat(TimelineCurve, UpdateFunctionFloat);
		Timeline->SetTimelineFinishedFunc(FinishedEvent);

		// 设置循环播放并从头开始播放
		Timeline->SetLooping(true);
		Timeline->PlayFromStart();
	}

}

// Called every frame
void AMySphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline)
	{
		// 手动驱动时间轴的 TickComponent 是可行的，但如果 Timeline 是 Actor 的子组件且已注册，
		// 引擎会自动为其 Tick。可以保留这行以保证在特殊情况下时间轴仍被驱动，
		// 也可以移除以依赖引擎自动调度。
		Timeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

}

void AMySphere::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->Implements<UBPI_Death>())
	{
		IBPI_Death::Execute_Death(OtherActor);
	}
}

void AMySphere::OnTimelineFloatUpdate(float Value)
{
	// 使用曲线数值驱动物体的 Roll 角度
	// 注意：Value 的含义取决于曲线资源（可能是 0..1、角度值等），请确保曲线的取值范围与期望相符
	// 如果希望做增量旋转而不是设置绝对角度，应使用 AddActorLocalRotation 或在曲线中返回累积值
	FRotator NewRotation = GetActorRotation();
	NewRotation.Roll = Value;
	SetActorRotation(NewRotation);
}

void AMySphere::OnTimelineFinished()
{
	// 时间轴完成时的回调，目前为空实现
	// 注意：如果设置了 SetLooping(true)，通常不会触发完成回调，除非先将循环关闭并到达结束

}

