// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGunner/OlofsCode/AI/StateMachine/FGAIStateBase.h"
#include "AIGunner/OlofsCode/AI/Sensing/FGVisionSensingComponent.h"
#include "HAI_Alert.generated.h"

class UFGNavMovementComponent;
class UEnemyGunComponent;
class AAIController;
class UFGVisionSensingComponent;
class AFGEnemy;

UCLASS()
class UHAI_Alert : public UFGAIStateBase
{
	GENERATED_BODY()
public:
	UHAI_Alert();

protected:
	AFGEnemy* OwnerEnemy;
	
	FVector LastKnownPosition = FVector::ZeroVector;
	AActor* Player = nullptr;
	
	UFUNCTION()
	void HandleSeePlayer(const FFGVisionSensingResults& Result);
	UFUNCTION()
	void HandleLostPlayer(const FFGVisionSensingResults& Results);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeUntilPatrol = 5.f;
	float TimeWithoutTarget = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AlertMovementSpeedMultiplier = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DesiredDistanceFromPlayer = 300.f;

	const float CloseEnoughDistance = 150.f; //If distance to destination is less than this we reach the point
	//Whether its olofs or the AIcontrollers movement, it seems a bit inaccurate, as the enemy gets stuck short distances away
	//This is a fix for that

public:
	virtual void Setup() override;
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual FName TickActive(float DeltaTime) override;
	virtual void OnNoiseHeard(const FNoiseInfo& NoiseInfo) override;
};
