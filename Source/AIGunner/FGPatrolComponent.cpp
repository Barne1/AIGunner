
#include "FGPatrolComponent.h"
#include "AIController.h"
#include "AIGunner/OlofsCode/AI/Movement/FGNavMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UFGPatrolComponent::UFGPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	RunSetup();
	if(bPatrolRequestedOnStart)
		StartPatrol();
}

void UFGPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bPatrolling)
	{
		RotateTowardsMovementDirection(MovComponent, DeltaTime);
		float SqrDistanceToDestination = FVector::DistSquared2D(NextDestination, OwnerPawn->GetActorLocation());
		if(Controller->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			WaitTimeRemaining -= DeltaTime;
			if(WaitTimeRemaining < 0)
			{
				SetNextPatrolDestination();
			}
		}
	}
}

//This method exists to safe against the state machine running StartPatrol in BeginPlay() before the patrol comp BeginPlay()
void UFGPatrolComponent::RunSetup()
{
	OwnerPawn = Cast<APawn>(GetOwner());
	Controller = Cast<AAIController>(OwnerPawn->Controller);
	MovComponent = Cast<UFGNavMovementComponent>(OwnerPawn->GetComponentByClass(UFGNavMovementComponent::StaticClass()));
	bSetupComplete = true;
}

void UFGPatrolComponent::StartPatrol()
{
	if(!bSetupComplete)
	{
		bPatrolRequestedOnStart = true;
		return;
	}
	
	if(PatrolPoints.Num() > 1)
	{
		CurrentPatrolIndex = 0;
		bPatrolling = true;
		NextDestination = PatrolPoints[0]->GetActorLocation();
		WaitTimeRemaining = WaitTimeAtPatrolPoint;
		Controller->MoveToLocation(NextDestination);
	}
}

void UFGPatrolComponent::StopPatrol()
{
	bPatrolling = false;
	Controller->StopMovement();
}

void UFGPatrolComponent::SetNextPatrolDestination()
{
	WaitTimeRemaining = WaitTimeAtPatrolPoint;
	
	CurrentPatrolIndex = ++CurrentPatrolIndex % PatrolPoints.Num(); //Can never overflow
	NextDestination = PatrolPoints[CurrentPatrolIndex]->GetActorLocation();
	
	Controller->MoveToLocation(NextDestination);
}

void UFGPatrolComponent::RotateTowardsMovementDirection(const UFGNavMovementComponent* MovementComponent, float DeltaTime)
{
	FVector Velocity = MovementComponent->RequestedVelocity;
	if(!Velocity.IsNearlyZero())
	{
		Velocity.Z = 0.f;
		FRotator Rot = UKismetMathLibrary::MakeRotFromX(Velocity);
		Rot = FMath::RInterpTo(OwnerPawn->GetActorRotation(), Rot, DeltaTime, RotationSpeed);
		GetOwner()->SetActorRotation(Rot);
	}
}



