// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ThirdCharacter.h"
#include "MySphere.generated.h"

class UTimelineComponent;
class UCurveFloat;
class USphereComponent;
class UStaticMeshComponent;
class AThirdCharacter;

UCLASS()
class ERZHANCHALLENGEGAME_API AMySphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category = Timeline)
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY(EditAnywhere,Category = Timeline)
	TObjectPtr<UCurveFloat> TimelineCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Sphere)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sphere)
	TObjectPtr<USphereComponent> SphereCollision;

	
	FOnTimelineFloat UpdateFunctionFloat;


	FOnTimelineEvent FinishedEvent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:

	UFUNCTION()
	void OnTimelineFloatUpdate(float Value);

	UFUNCTION()
	void OnTimelineFinished();
};
