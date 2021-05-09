// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyGunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireDelegate, FVector, ShotStart, FVector, ShotEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIGUNNER_API UEnemyGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyGunComponent();

protected:
	virtual void BeginPlay() override;

	void Fire(const AActor* ShotTarget);

	UPROPERTY(BlueprintReadOnly, Category="Gun")
	bool bIsFiring = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	int Damage = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	float TimeBetweenShots = 1.f;
	float TimeToNextShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	float StartingAccuracyPenalty = 1;
	float AccuracyPenalty;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	float AccuracyPenaltyDecrease = 0.2f;
	
	FVector GetAimingDirection(FVector TargetPosition, FVector Start);
public:
	void BeginFire(AActor* NewTarget);
	void StopFire();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FFireDelegate OnFiredShot;

	UFUNCTION(BlueprintCallable)
	void SetupComponent(AActor* WeaponActor);
	UStaticMeshComponent* GunMeshComp;
	const FName MuzzleSocketName = "Muzzle";

	void IncreaseAccuracy();
	void DecreaseAccuracy();

	UPROPERTY(EditDefaultsOnly, Category="Gun")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(BlueprintReadOnly)
	AActor* Target;
};
