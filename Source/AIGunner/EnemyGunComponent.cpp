// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGunComponent.h"


#include "FGHealthComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIGunner/OlofsCode/FGCharacter.h"

// Sets default values for this component's properties
UEnemyGunComponent::UEnemyGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEnemyGunComponent::BeginPlay()
{
	Super::BeginPlay();

	TimeToNextShot = TimeBetweenShots; //Some wind-up time until the first shot
	AccuracyPenalty = StartingAccuracyPenalty;
	
}

void UEnemyGunComponent::Fire(const AActor* ShotTarget)
{
	FVector Start = GunMeshComp->GetSocketLocation(MuzzleSocketName);
	FVector RelativeEnd = GetAimingDirection(ShotTarget->GetActorLocation(), Start) * 10000.f;
	FVector End = Start + RelativeEnd;
	
	FHitResult Hit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceChannel, true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);
	FVector ShotEndPoint = End;
	if(Hit.bBlockingHit)
	{
		End = Hit.ImpactPoint;
		AFGCharacter* Player = Cast<AFGCharacter>(Hit.Actor);
		if(Player)
		{
			UActorComponent* UncastedComp = Player->GetComponentByClass(UFGHealthComponent::StaticClass());
			ensure(UncastedComp != nullptr);
			Cast<UFGHealthComponent>(UncastedComp)->TakeDamage(Damage);
		}
	}
	OnFiredShot.Broadcast(Start, End);
}

FVector UEnemyGunComponent::GetAimingDirection(FVector TargetPosition, FVector Start)
{
	FVector DirectionToTarget = TargetPosition - Start;
	FVector UpDirection = FVector::UpVector;
	FVector SideDirection = FVector::CrossProduct(DirectionToTarget.GetSafeNormal(), UpDirection);

	FVector UpAccuracyPenalty = UpDirection * FMath::RandRange(-AccuracyPenalty, AccuracyPenalty);
	FVector SideAccuracyPenalty = SideDirection * FMath::RandRange(-AccuracyPenalty, AccuracyPenalty);
	
	return ((TargetPosition + UpAccuracyPenalty + SideAccuracyPenalty)-Start).GetSafeNormal();
}

void UEnemyGunComponent::BeginFire(AActor* NewTarget)
{
	Target = NewTarget;
	bIsFiring = true;
}

void UEnemyGunComponent::StopFire()
{
	Target = nullptr;
	bIsFiring = false;
}

void UEnemyGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsFiring)
	{
		TimeToNextShot -= DeltaTime;
		if(TimeToNextShot < 0 && Target != nullptr) //Target should never be null at the same time as bIsFiring, but better safe than sorry in case the designer adds functionality in blueprint
		{
			Fire(Target);
			IncreaseAccuracy();
			TimeToNextShot = TimeBetweenShots;
		}
		
	}
	else
	{
		TimeToNextShot += DeltaTime;
		TimeToNextShot = FMath::Min(TimeBetweenShots, TimeToNextShot);
		DecreaseAccuracy();
	}
}

void UEnemyGunComponent::SetupComponent(AActor* WeaponActor)
{
	UActorComponent* UncastedMeshComp = WeaponActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
	ensure(UncastedMeshComp != nullptr);

	GunMeshComp = Cast<UStaticMeshComponent>(UncastedMeshComp);
}

void UEnemyGunComponent::IncreaseAccuracy()
{
	AccuracyPenalty -= AccuracyPenaltyDecrease;
	AccuracyPenalty = FMath::Max(0.f, AccuracyPenalty);
}

void UEnemyGunComponent::DecreaseAccuracy()
{
	AccuracyPenalty += AccuracyPenaltyDecrease;
	AccuracyPenalty = FMath::Min(StartingAccuracyPenalty, AccuracyPenalty);
}



