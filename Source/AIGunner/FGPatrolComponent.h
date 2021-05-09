// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGPatrolComponent.generated.h"

class AAIController;
class UFGNavMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIGUNNER_API UFGPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFGPatrolComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	AAIController* Controller;
	APawn* OwnerPawn;
	UFGNavMovementComponent* MovComponent;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere)
	float WaitTimeAtPatrolPoint = 2.f;
	
	float WaitTimeRemaining = 0;
	
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 0.1f;

	int CurrentPatrolIndex = 0;
	FVector NextDestination;
	bool bPatrolling;

	bool bSetupComplete = false; //checks if we have setup owner pawn and movcomp properly
	bool bPatrolRequestedOnStart = false; //Checks if startpatrol was called before beginplay
	void RunSetup();
public:
	UFUNCTION(BlueprintCallable)
	void StartPatrol();
	UFUNCTION(BlueprintCallable)
    void StopPatrol();

	UFUNCTION(BlueprintCallable)
	void SetNextPatrolDestination();

	void RotateTowardsMovementDirection(const UFGNavMovementComponent* MovementComponent, float DeltaTime);
};
