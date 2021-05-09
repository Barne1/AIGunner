#include "FGVisionSensingComponent.h"
#include "FGVisionSensingSettings.h"
#include "FGVisionSensingTargetComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UFGVisionSensingComponent::UFGVisionSensingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGVisionSensingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bActivated)
		return;
	
	if (SensingSettings == nullptr)
		return;

	const FVector Direction = GetOwner()->GetActorForwardVector();
	const FVector Origin = GetOwner()->GetActorLocation();

	if (bDebugDrawVision)
	{
		FVector Right = Direction.RotateAngleAxis(SensingSettings->Angle, FVector::UpVector);
		FVector Left = Direction.RotateAngleAxis(-SensingSettings->Angle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SensingSettings->DistanceMinimum, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SensingSettings->DistanceMinimum, FLinearColor::Green);
	}

	bCanSeePlayer = false;
	for (int32 Index = SensedTargets.Num() - 1; Index >= 0; --Index)
	{
		UFGVisionSensingTargetComponent* Target = SensedTargets[Index];

		if (Target == nullptr || (Target != nullptr && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(Index);
			continue;
		}
		
		//Addition
		if(Target->bIsPlayer)
		{
			bCanSeePlayer = true; //used to decay timer before alert
			if(!bAlerted) //prevent constant calls to OnAlert
			{
				TimeVisible += DeltaTime;
				if(TimeVisible >= TimeUntilAlert)
				{
					FFGVisionSensingResults Results;
					Results.SensedActor = Target->GetOwner();
					OnAlert.Broadcast(Results);
					OnPlayerSeen.Broadcast(Results); //Needed since player might be seen before alert
					LastSeenPosition = Results.SensedActor->GetActorLocation();
					bAlerted = true;
				}
				else if(TimeVisible >= TimeUntilSuspicious)
				{
					FFGVisionSensingResults Results;
					Results.SensedActor = Target->GetOwner();
					OnSuspicious.Broadcast(Results);
					LastSeenPosition = Results.SensedActor->GetActorLocation();
				}
			}
		}

		if (!IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum,GetOwner(), Target->GetOwner()))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			//OnTargetLost.Broadcast(Results);
			if(Target->bIsPlayer)
				OnPlayerLost.Broadcast(Results);
			SensedTargets.RemoveAt(Index);
		}
	}

	//Addition
	if(!bCanSeePlayer)
	{
		TimeVisible -= DeltaTime;
		TimeVisible = FMath::Max(0.f, TimeVisible);
	}

	TArray<UFGVisionSensingTargetComponent*> ListOfTargets;
	
	UFGVisionSensingTargetComponent::GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), SensingSettings->DistanceMinimum);

	for (UFGVisionSensingTargetComponent* Target : ListOfTargets)
	{
		if (!SensedTargets.Contains(Target) &&
		IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum, GetOwner(), Target->GetOwner()))
		{
			SensedTargets.Add(Target);
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			//OnTargetSensed.Broadcast(Results);
			if(Target->bIsPlayer)
				OnPlayerSeen.Broadcast(Results);
		}
	}
}

bool UFGVisionSensingComponent::IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum, AActor* Senser, AActor* Sensed) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq)
	{
		return false;
	}

	if(BlockedByWall(Senser, Sensed))
		return false;

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	const bool bIsValid = Dot > AsHalfRad;

	return bIsValid;
}

float UFGVisionSensingComponent::GetVisionInRadians() const
{
	if (SensingSettings == nullptr)
		return 0.0;

	return FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
}

bool UFGVisionSensingComponent::BlockedByWall(AActor* Senser, AActor* Sensed) const
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Senser);
	ActorsToIgnore.Add(Sensed);

	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this, Senser->GetActorLocation(), Sensed->GetActorLocation(), VisionTraceChannel, true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	if(Hit.bBlockingHit)
	{
		return true;
	}

	return false;
}

float UFGVisionSensingComponent::GetPercentAlerted()
{
	return (TimeVisible/TimeUntilAlert);
}

float UFGVisionSensingComponent::GetPercentSuspicious()
{
	return (TimeVisible/TimeUntilSuspicious);
}

void UFGVisionSensingComponent::DisableVision()
{
	bActivated = false;
}

void UFGVisionSensingComponent::EnableVision()
{
	bActivated = true;
}
