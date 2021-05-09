// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGunner/HugoStates/HAI_Alert.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIGunner/EnemyGunComponent.h"
#include "AIGunner/OlofsCode/FGEnemy.h"
#include "AIGunner/OlofsCode/AI/Movement/FGNavMovementComponent.h"
#include "AIGunner/OlofsCode/AI/Sensing/FGHearingComponent.h"

UHAI_Alert::UHAI_Alert()
{
	StateName = "Alert";
	PendingStateName = NAME_None; //Not sure if this is needed
}

void UHAI_Alert::HandleSeePlayer(const FFGVisionSensingResults& Results)
{
	OwnerEnemy->AiController->StopMovement();
	Player = Results.SensedActor;
}

void UHAI_Alert::HandleLostPlayer(const FFGVisionSensingResults& Results)
{
	Player = nullptr;
	LastKnownPosition = Results.SensedActor->GetActorLocation();
	OwnerEnemy->AiController->MoveToLocation(LastKnownPosition);
}

void UHAI_Alert::Setup()
{
	OwnerEnemy = Cast<AFGEnemy>(Owner);
	ensure(OwnerEnemy != nullptr);

	Super::Setup();
}

void UHAI_Alert::OnActivated()
{
	OwnerEnemy->VisionSensingComponent->OnPlayerSeen.AddDynamic(this, &UHAI_Alert::HandleSeePlayer);
	OwnerEnemy->VisionSensingComponent->OnPlayerLost.AddDynamic(this, &UHAI_Alert::HandleLostPlayer);

	OwnerEnemy->NavMovementComponent->IncreaseMovementSpeed(AlertMovementSpeedMultiplier);

	if(!OwnerEnemy->VisionSensingComponent->bCanSeePlayer)
	{
		//TODO: potential null ref in the future, but currently can only enter alert by hearing noise or seeing the enemy
		LastKnownPosition = OwnerEnemy->HearingComponent->LastNoiseHeardInfo->Location;
		OwnerEnemy->AiController->MoveToLocation(LastKnownPosition);
	}

	Super::OnActivated();
}

void UHAI_Alert::OnDeactivated()
{
	OwnerEnemy->VisionSensingComponent->OnPlayerSeen.RemoveDynamic(this, &UHAI_Alert::HandleSeePlayer);
	OwnerEnemy->VisionSensingComponent->OnPlayerLost.RemoveDynamic(this, &UHAI_Alert::HandleLostPlayer);
	OwnerEnemy->GunComponent->StopFire();
	
	TimeWithoutTarget = 0;
	Player = nullptr;
	LastKnownPosition = FVector::ZeroVector;

	//Todo move this to a method in FGNavMovementComponent
	OwnerEnemy->NavMovementComponent->ResetMovementSpeed();

	Super::OnDeactivated();
}

FName UHAI_Alert::TickActive(float DeltaTime)
{
	if(Player != nullptr)
	{
		FVector VectorToPlayer = Player->GetActorLocation() - Owner->GetActorLocation();
		VectorToPlayer.Z = 0; //floored
		FRotator Rot = UKismetMathLibrary::MakeRotFromX(VectorToPlayer);
		Owner->SetActorRotation(Rot);

		float SqrDistance = VectorToPlayer.SizeSquared();
		float SqrDesiredDistance = DesiredDistanceFromPlayer * DesiredDistanceFromPlayer;
		
		if(SqrDistance < SqrDesiredDistance)
		{
			OwnerEnemy->GunComponent->BeginFire(Player);
		}
		else
		{
			OwnerEnemy->GunComponent->StopFire();
			float Distance = FMath::Sqrt(SqrDistance);
			float DistanceToMove = Distance - (DesiredDistanceFromPlayer-CloseEnoughDistance);
			FVector Destination = Owner->GetActorLocation() + (VectorToPlayer.GetSafeNormal() * DistanceToMove);
			OwnerEnemy->AiController->MoveToLocation(Destination);
		}
	}
	else
	{
		OwnerEnemy->GunComponent->StopFire();
		FVector DirectionToLast = LastKnownPosition - OwnerEnemy->GetActorLocation();
		DirectionToLast.Z = 0;
		FRotator NewRot = UKismetMathLibrary::MakeRotFromX(DirectionToLast);
		OwnerEnemy->SetActorRotation(NewRot);
		if(LastKnownPosition == FVector::ZeroVector || DirectionToLast.Size2D() < CloseEnoughDistance) //Close enough not squared, but works fine anyway so no need for extra steps
		{
			TimeWithoutTarget += DeltaTime;
			if(TimeWithoutTarget > TimeUntilPatrol)
			{
				//Todo make investigate state
				//Todo rotate back and forth to scan area
				PendingStateName = "IdlePatrol";
			}
		}
		
	}
	return Super::TickActive(DeltaTime);
}

void UHAI_Alert::OnNoiseHeard(const FNoiseInfo& NoiseInfo)
{
	//Todo: figure out a better system to tell the ai if its a player shot, maybe a bool or enum in FNoiseInfo?
	if(NoiseInfo.ReactionPriority > 5) //If its a player shot
	{
		LastKnownPosition = NoiseInfo.Location;
		OwnerEnemy->AiController->MoveToLocation(LastKnownPosition);
	}
}

